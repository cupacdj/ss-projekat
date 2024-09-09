// Siglton class
class Assembler
{
public:
    static Assembler &getInstance()
    {
        static Assembler instance;
        return instance;
    }
    Assembler(Assembler const &) = delete;
    void operator=(Assembler const &) = delete;

private:
    Assembler() {}
};