#include "FrameworkPCH.h"
#include "Log.h"

namespace fw
{

DebugLog log;

Log::Log(const char* fileName)
{
    m_FileName = fileName;
    // Opens the log file into a file stream
    m_FileStream.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
}

Log::~Log()
{
    // Dots to separate each session's log texts.
    m_FileStream << "....................................\n";
    m_FileStream << "\n";

    //Writes the texts from the Log into log file stream
    m_FileStream << log.Buf.c_str();

    // Closes the log file stream
    m_FileStream.close();
}

}