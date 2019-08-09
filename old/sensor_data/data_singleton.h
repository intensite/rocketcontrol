class DataSingleton
{
    private:
        int mData;

    public:
    
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
        float alti;
        bool is_abort = false;
        bool is_apogee = false;

        static DataSingleton& getInstance()
        {
            static DataSingleton instance;
            return instance;
        }
        int GetData()
        {
            return mData;
        }
    private:
        DataSingleton() {};
        DataSingleton(DataSingleton const&);
        void operator=(DataSingleton const&);
};