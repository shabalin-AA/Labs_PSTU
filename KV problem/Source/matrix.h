#pragma once
#define inf 100
#define max_size 20

class matrix : public juce::Component //класс матрицыы смежности
{
public:
    matrix()
    {
        setSize(Width, Height);
    }

    ~matrix() {}

    void Copy (matrix& other) //копирование одной матрицы в другую
    {
        for (int i=0; i<currentSize; i++)
            for (int j=0; j<currentSize; j++)
                setValue(i, j, other.getValue(i, j));
    }

    void paint (juce::Graphics& g) override //отрисовка матрицы
    {
        int x = 0, y = 0, w, h;
        if (currentSize > 0) //ширина и высота окошек под текст определяются в зависимости от количества элементов в матрице
        {
            w = Width  / currentSize;
            h = Height / currentSize;
        }
        else
        {
            w = 0; h = 0;
        }

        g.setColour(juce::Colours::white);

        for (int i=0; i<currentSize; i++) //отрисовка занчений матрицы
        {
            for (int j=0; j<currentSize; j++)
            {
                if (M[i][j] == inf)
                {
                    juce::String s {"inf"};
                    g.drawText (s,
                        x + w*j, y + h*i, w, h,
                        juce::Justification::centred);
                }
                else
                {
                    g.drawText (juce::String(M[i][j]),
                        x + w*j, y + h*i, w, h,
                        juce::Justification::centred);
                }
            }
        }

    }

    void setValue (int Ln, int Col, int Value) //селекторы и модификаторы для полей матрицы
    {
        M[Ln][Col] = Value;
    }

    int getValue (int x, int y)
    {
        return M[x][y];
    }

    int getCurrentSize () { return currentSize; }

    void setCurrentSize (int newSize)
    {
        if (newSize <= max_size)
            currentSize = newSize;
    }

    void nullGenerate ()
    {
        for (int i=0; i<max_size; i++)
            for (int j=0; j<max_size; j++)
                M[i][j] = inf;
    }

    int getMyWidth ()  { return Width; }
    int getMyHeight () { return Height; }

private:
    int M [max_size][max_size];
    int currentSize = 0;

    int Width = 220, Height = 200;
};
