template<typename T>
class Singleton
{
protected:
    Singleton() = default;
public:
      static T& Instance()
        {
            static T instance;
            return instance;
        }
}