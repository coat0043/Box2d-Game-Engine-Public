#pragma once

/* Debug Logging into the Log window.
*  Formatting Specifiers:
*  %s = String,		%f = float,	  %0.2f = float with 2 decimals		%d = int
*  Example: LOG(WARNING, "PlayerPosition: vec2(%0.2f, %0.2f)", playerPos.x, playerPos.y);
*  Also see the Macros at the bottom
*/

namespace fw
{

struct DebugLog
{
    ImGuiTextBuffer     Buf;		 // Text buffer
    ImVector<int>       LineOffsets; // Index to lines offset.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    DebugLog()
    {
        AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();

        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step())
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

                /*Changes the color of the log message based on the Log Type.*/
                // [Info] = White
                if (line_start[1] == 'I')
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                }
                // [Warning] = Yellow
                else if (line_start[1] == 'W')
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                }
                // [Error] = Red
                else if (line_start[1] == 'E')
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                }

                ImGui::TextUnformatted(line_start, line_end);
                ImGui::PopStyleColor();
            }
        }
        clipper.End();

        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};

extern DebugLog log;

enum class LogType
{
    Info,
    Error,
    Warning,
};

class Log
{
public:
    Log(const char* fileName = "Log.txt");
    ~Log();

    inline static void NextLine()
    {
        log.AddLog("\n");
    }

    // Adds the Log Type at the beginning of the Log Message
    inline static void AddType(LogType type)
    {
        if (type == LogType::Info)
        {
            log.AddLog("[Info] ");
        }
        else if (type == LogType::Error)
        {
            log.AddLog("[Error] ");
        }
        else if (type == LogType::Warning)
        {
            log.AddLog("[Warning] ");
        }
    }

    inline static void Update(bool* p_open = NULL)
    {
        // Creates a window named Log
        ImGui::Begin("Log", p_open);

        // Creates a log window
        log.Draw("Log", p_open);

        // End: Log Window
        ImGui::End();
    }

    const char* GetFileName() { return m_FileName; }

private:

    const char* m_FileName;
    std::fstream m_FileStream;

};

} // fw


// Debug Config only
#ifdef _DEBUG
/*
*	Logs a message in the Log Window.
*	Example Usage 1: LOG(INFO, "PlayerPos: %f, %f \n", pPlayer->GetPosition().x, pPlayer->GetPosition().y);
*	Example Usage 2: LOG(WARNING, "E key is pressed");													*/
#define LOG(x, ...)		   fw::Log::AddType(x), fw::log.AddLog(__VA_ARGS__), fw::Log::NextLine()

// Clears the Log Window.
#define CLEAR_LOG		   fw::log.Clear()

//---------  Log Types  ----------//
// Logs the message as WHITE. Used for casual logs.
#define INFO			   fw::LogType::Info

// Logs the message as RED.
#undef ERROR
#define ERROR			   fw::LogType::Error

// Logs the message as YELLOW.
#define WARNING			   fw::LogType::Warning

#endif // _DEBUG
