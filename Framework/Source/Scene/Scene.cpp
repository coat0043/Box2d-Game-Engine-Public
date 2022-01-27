#include "FrameworkPCH.h"

#include "EventSystem/EventManager.h"

#include "Scene.h"
#include "Components/AABBComponent.h"
#include "Components/ComponentManager.h"
#include "Components/ComponentRegistry.h"
#include "Components/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/TransformComponent.h"
#include "Core/GameCore.h"
#include "EventSystem/Event.h"
#include "Lua/LuaGameState.h"
#include "Lua/LuaScript.h"
#include "Objects/AABB.h"
#include "Objects/GameObject.h"
#include "Objects/Material.h"
#include "Objects/ResourceManager.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsWorld2D.h"
#include "UI/Log.h"
#include "Utility/JSONHelpers.h"
#include "Utility/Helpers.h"
namespace fw {

Scene::Scene(GameCore* pGameCore)
{
    m_pGameCore = pGameCore;

    m_pComponentManager = new ComponentManager();

    m_pGameCore->GetEventManager()->RegisterEventListener(this, RemoveFromGameEvent::GetStaticEventType());
    m_pGameCore->GetEventManager()->RegisterEventListener(this, BeginPlayEvent::GetStaticEventType());
    m_pGameCore->GetEventManager()->RegisterEventListener(this, EndPlayEvent::GetStaticEventType());
}

Scene::~Scene()
{
    for (GameObject* pObject : m_Objects)
    {
        delete pObject;
    }

    delete m_pWorld;

    delete m_pComponentManager;

#if FW_USING_LUA
    delete m_pLuaGameState;
#endif

    delete m_pMousePickerMaterial;

    m_pGameCore->GetEventManager()->UnregisterEventListenerCompletely(this);

    //assert( m_pGameCore->GetEventManager()->AreAnyListenersRegistered() == false );
}

void Scene::OnEvent(Event* pEvent)
{
    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find(m_Objects.begin(), m_Objects.end(), pObject);
        m_Objects.erase(it);

        delete pObject;
    }
    else if (pEvent->GetType() == BeginPlayEvent::GetStaticEventType())
    {
        SaveToFile("temp.scene");
    }
    else if (pEvent->GetType() == EndPlayEvent::GetStaticEventType())
    {
        LoadFromFile("temp.scene");
    }
}

void Scene::Init()
{
    if (m_pWorld == nullptr)
    {
        m_pWorld = new PhysicsWorld2D(m_pGameCore->GetEventManager());
    }
    m_pWorld->SetCanSleep(true);
    m_pWorld->SetGravity(vec3(0, 0, 0));

    m_pMousePickerMaterial = new fw::Material(m_pGameCore->GetResourceManager()->GetShader("Color"), fw::Color::Red());
}

void Scene::Update(float deltaTime)
{
    if (m_pWorld)
    {
        m_pWorld->Update(deltaTime);
    }

    for (GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    GetComponentManager()->Update(deltaTime);

    if (m_pActiveCameraComponent)
    {
        vec2 mousePos = m_pActiveCameraComponent->GetNormalizedDeviceCoordinates();
    }
}

void Scene::Draw()
{
    if (m_pActiveCameraComponent == nullptr)
        return;

    GetComponentManager()->Draw(m_pActiveCameraComponent);

    glClear(GL_DEPTH_BUFFER_BIT);

    if (m_pWorld && m_pGameCore->GetDrawPhysicsDebugShapes())
    {
        m_pWorld->DrawDebugData(m_pActiveCameraComponent, m_Is2DPhysicsOnXZPlane);
    }
}

void Scene::DrawMousePickingMeshes()
{
    int pickingID = -1;

    std::vector<Component*>& list = GetComponentManager()->GetComponentList(MeshComponent::GetStaticType());
    for (int i = 0; i < list.size(); i++)
    {
        pickingID += 1; //Change to increment by 1 only

        m_pMousePickerMaterial->SetColor(Color(pickingID / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f));

        MeshComponent* pMeshComponent = static_cast<MeshComponent*>(list[i]);

        GameObject* pGO = pMeshComponent->GetGameObject();

        if (pGO != nullptr)
        {
            if (pMeshComponent->GetMesh() && pGO->GetTransform() && m_pActiveCameraComponent)
            {
                pMeshComponent->GetMesh()->Draw(m_pActiveCameraComponent,
                    pGO->GetTransform()->GetWorldTransform(), m_pMousePickerMaterial);
            }
        }
    }
}

void Scene::SelectObject(int selectedObject)
{
    m_ImGuiSelectedObject = nullptr;
    std::vector<Component*>& list = GetComponentManager()->GetComponentList(MeshComponent::GetStaticType());
    for (int i = 0; i < list.size(); i++)
    {
        if (selectedObject == i)
        {
            m_ImGuiSelectedObject = list[i]->GetGameObject();
            m_IsObjectInspectorOn = true;
        }
    }
}

void Scene::ClearScene()
{
    m_ImGuiSelectedObject = nullptr;

    for (size_t i = 0; i < m_Objects.size(); i++)
    {
        delete m_Objects[i];
    }
    m_Objects.clear();
}

GameObject* Scene::GetGameObjectByName(std::string name)
{
    for (auto& object : m_Objects)
    {
        if (object->GetName() == name)
            return object;
    }

    LOG(ERROR, "GameObject not found");
    return nullptr;
}

void Scene::DrawObjectList()
{
    if (m_IsObjectListOn == true)
    {
        ImGui::Begin("Object List", &m_IsObjectListOn);
        {
            ImGui::PushID("Scene Name");
            // Sets the color of the Scene Name at the top of the list.
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scene: %s", GetName().c_str());

            // Opens a popup menu when Name is right-clicked.
            if (ImGui::BeginPopupContextItem("Object Selection Menu"))
            {
                // Changes Scene Name.
                ImGui::Text("Rename Scene:");

                std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
                size_t clipSize = clipText.length();
                // Size of string.
                const size_t size = 32;
                char newText[size];
                strncpy_s(newText, size, m_Name.c_str(), sizeof(newText));

                ImGui::InputText("", newText, size);
                SetName(newText);

                ImGui::Separator();

                ImGui::EndPopup();
            }
            ImGui::PopID();
        }

        ImGui::Separator();

        ImGui::Text("Objects:");

        // Arranges Objects vector into a selectable list.
        bool objectHandledRightClick = false;
        if (ImGui::BeginListBox("Objects", ImVec2(-1, -1)))
        {
            int n = 0;
            for (int i = 0; i < m_Objects.size(); i++)
            {
                GameObject* pObject = pObject = m_Objects[i];

                if (pObject->GetTransform() == nullptr || pObject->GetTransform()->GetParent() == nullptr)
                {
                    ImGui::PushID(pObject);
                    if (DrawObject(pObject))
                    {
                        objectHandledRightClick = true;
                    }
                    ImGui::PopID();
                }
            }

            ImGui::EndListBox();
        }

        if (objectHandledRightClick == false)
        {
            // Opens a popup menu when Name is right-clicked.
            if (ImGui::BeginPopupContextItem("Object Selection Menu"))
            {
                // Displays list of buttons to add objects, located in Child Scene.
                DrawNewObjectButtons();
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}

bool Scene::DrawObject(GameObject* pObject)
{
    bool handledRightClick = false;

    ImGui::PushID(pObject);

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;

    bool hasChildren = false;
    if (pObject->GetTransform())
    {
        std::vector<GameObject*>& children = pObject->GetTransform()->GetChildren();
        if (children.size() > 0)
        {
            hasChildren = true;
            node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        }
    }

    const bool isSelected = (m_ImGuiSelectedObject == pObject);

    if (isSelected)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool node_open = ImGui::TreeNodeEx(pObject, node_flags, pObject->GetName().c_str());
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsItemClicked())
        {
            m_ImGuiSelectedObject = pObject;
            m_IsObjectInspectorOn = true;
        }
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObjectPtr", &pObject, sizeof(GameObject*));
        ImGui::Text(pObject->GetName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (pObject->GetTransform())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectPtr"))
            {
                assert(payload->DataSize == sizeof(GameObject*));
                GameObject* pDroppedObject = *(GameObject**)payload->Data;

                // If object being dropped is already not a child.
                if (pDroppedObject->GetTransform() &&
                    pDroppedObject->GetTransform()->GetParent() != pObject)
                {
                    pObject->GetTransform()->SetChild(pDroppedObject);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    // Opens a popup menu when Name is right-clicked.
    if (ImGui::BeginPopupContextItem("Object Options"))
    {
        handledRightClick = true;

        m_ImGuiSelectedObject = pObject;

        //// Changes Object Name.
        //ImGui::Text("Rename:");

        //std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
        //size_t clipSize = clipText.length();
        //const size_t size = 32;
        //char newText[size];
        //strncpy_s(newText, size, pObject->GetName().c_str(), sizeof(newText));

        //ImGui::InputText("", newText, size);
        //pObject->SetName(newText);

        //ImGui::Separator();

        if (pObject->GetTransform() && pObject->GetTransform()->GetParent())
        {
            if (ImGui::MenuItem("Detach"))
            {
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Detach this object from its parent");

                pObject->GetTransform()->SetParent(nullptr);
            }
        }

        if (ImGui::MenuItem("Duplicate"))
        {
            GameObject* newObj = new GameObject(*pObject);

            m_Objects.push_back(newObj);
        }

        ImGui::Separator();

        // Deletes Object.
        if (ImGui::MenuItem("Delete Object"))
        {
            m_pGameCore->GetEventManager()->AddEvent(new RemoveFromGameEvent(pObject));
            m_ImGuiSelectedObject = nullptr;

            LOG(INFO, "%s successfully deleted from Scene", pObject->GetName().c_str());
        }

        ImGui::Separator();

        // List of Components to Add
        // UI for adding components to game objects
        m_pGameCore->GetComponentRegistry()->AddInterfaceToCreateComponents(this, pObject);

        ImGui::EndPopup();
    }

    if (node_open)
    {
        if (hasChildren)
        {
            std::vector<GameObject*>& children = pObject->GetTransform()->GetChildren();

            for (int i = 0; i < children.size(); i++)
            {
                if (DrawObject(children[i]))
                    handledRightClick = true;
            }
        }
        ImGui::TreePop();
    }

    ImGui::PopID();

    return handledRightClick;
}

void Scene::DrawNewObjectButtons()
{
}

void Scene::DrawImguiDemoWindow()
{
    if (m_IsImguiDemoWindowOn == true)
    {
        ImGui::ShowDemoWindow();
    }
}

void Scene::LoadFromFile(std::string path, std::string sceneName)
{
    if (LoadFromFile(path + "/" + sceneName))
    {
        SetPath(path);
        SetName(sceneName);
    }
}

bool Scene::LoadFromFile(std::string filename)
{
    ClearScene();

    std::filesystem::path relativePath = filename;
    const char* jsonSceneFile = LoadCompleteFile(relativePath.u8string().c_str(), nullptr);
    if (jsonSceneFile == nullptr)
        return false;

    rapidjson::Document loadedScene;
    rapidjson::ParseResult ok = loadedScene.Parse(jsonSceneFile);
    delete[] jsonSceneFile;

    if (ok == false)
        return false;

    if (loadedScene.HasMember("PhysicsWorld"))
    {
        rapidjson::Value& physicsWorld = loadedScene["PhysicsWorld"];
        if (physicsWorld.HasMember("Exists"))
        {
            if ((bool)physicsWorld["Exists"].GetBool() == true)
            {
                if (m_pWorld == nullptr)
                {
                    m_pWorld = new PhysicsWorld2D(m_pGameCore->GetEventManager());
                }
                m_pWorld->SetCanSleep(true);
                if (physicsWorld.HasMember("Gravity"))
                {
                    m_pWorld->SetGravity(vec3(0, 0, 0));
                }
            }
        }

        JSONLoadBool(physicsWorld, "XZPlane", &m_Is2DPhysicsOnXZPlane);
    }

    for (rapidjson::Value& gameObject : loadedScene["GameObjects"].GetArray())
    {
        std::string objectName = gameObject["Name"].GetString();

        GameObject* pGameObject = new GameObject(this, objectName);
        m_Objects.push_back(pGameObject);
        pGameObject->Load(gameObject);
    }

    // Finalize Load.
    for (rapidjson::Value& gameObject : loadedScene["GameObjects"].GetArray())
    {
        std::string objectName = gameObject["Name"].GetString();
        GameObject* pGameObject = GetGameObjectByName(objectName);
        pGameObject->FinalizeLoad(gameObject);
    }

    return true;
}

void Scene::SaveToFile()
{
    SaveToFile(m_Path + "/" + m_Name);
}

void Scene::SaveToFile(std::string filename)
{
    rapidjson::StringBuffer buffer;
    WriterType writer(buffer);
    writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);

    writer.StartObject(); // Root json object
    {
        writer.Key("PhysicsWorld");
        writer.StartObject();
        {
            writer.Key("Exists");
            if (m_pWorld != nullptr)
            {
                writer.Bool(true);
                JSONSaveVec3(writer, "Gravity", m_pWorld->GetGravity());
            }
            else
            {
                writer.Bool(false);
            }

            JSONSaveBool(writer, "XZPlane", m_Is2DPhysicsOnXZPlane);
        }
        writer.EndObject();

        writer.Key("GameObjects");
        writer.StartArray();
        {
            for (GameObject* obj : m_Objects)
            {
                writer.StartObject();
                obj->Save(writer);
                writer.EndObject();
            }
        }
        writer.EndArray();
    }
    writer.EndObject(); // Root json object

    // Save json to disk.
    FILE* pFile = nullptr;
    fopen_s(&pFile, filename.c_str(), "wb");
    if (pFile != nullptr)
    {
        const char* str = buffer.GetString();
        int length = (int)strlen(str);
        fwrite(str, 1, length, pFile);
        fclose(pFile);
    }
}

void Scene::CheckAABBCollision()
{
    std::vector<Component*> pAABBComponents = GetComponentManager()->GetComponentList(AABBComponent::GetStaticType());

    for (int i = 0; i < pAABBComponents.size(); i++)
    {
        AABBComponent* pComponentI = static_cast<AABBComponent*>(pAABBComponents[i]);

        for (int j = i + 1; j < pAABBComponents.size(); j++)
        {
            AABBComponent* pComponentJ = static_cast<AABBComponent*>(pAABBComponents[j]);

            if (pComponentI->GetAABB().CheckCollision(pComponentJ->GetAABB()))
            {
                GetGameCore()->GetEventManager()->AddEvent(
                    new AABBCollisionEvent(pComponentI->GetGameObject(),
                    pComponentJ->GetGameObject()));
            }
        }
    }
}

void Scene::DrawImGuiInspector()
{
    if (m_IsObjectInspectorOn == true)
    {
        ImGui::Begin("Inspector", &m_IsObjectInspectorOn);

        if (m_ImGuiSelectedObject != nullptr)
        {
            ImGui::PushID(m_ImGuiSelectedObject);
            m_ImGuiSelectedObject->AddToInspector();
            ImGui::PopID();
        }
        else
        {
            ImGui::Text("Select an object to inspect...");
        }

        ImGui::End();
    }
}

} // namespace fw
