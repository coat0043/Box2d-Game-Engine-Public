#pragma once
#include "Math/MathHelpers.h"


namespace  fw
{
    struct WaveData;
    
    class Sound
    {
    public:
        Sound(float Importance, const char* filename);
        ~Sound();

        void SetWaveFormate();
        void CreateFromWaveFile(const char* filename);
        WaveData* GetWaveData() { return m_WaveData; }
        float GetImportance() { return m_Importance; }
        void SetWaveData(WaveData* pWaveData) { m_WaveData = pWaveData; }
        //Set Importance in range of 0.0f - 100.0f
        void SetImportance(float Importance) { m_Importance = MyClamp_Return(Importance, 0.0f, 100.0f); }
        //Set Weight in range of 0.0f - 100.0f
        void SetWeight(float Weight) { m_Weight = MyClamp_Return(Weight, 0.0f, 100.0f); }
        float GetWeight() { return m_Weight; }
        void SetAmountNotPicked(int AmountNotPicked) { m_AmountNotPicked = AmountNotPicked; }
        int GetAmountNotPicked() { return m_AmountNotPicked; }
        

    private:
        WaveData* m_WaveData;
        float m_Importance;
        float m_Weight;
        int m_AmountNotPicked; 
    };
} //namespace fw
