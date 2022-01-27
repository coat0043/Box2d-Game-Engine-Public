#include "FrameworkPCH.h"
#include "Component.h"

void fw::Component::Save(WriterType& writer)
{
    JSONSaveCharArray(writer, "Type", GetType());
}
