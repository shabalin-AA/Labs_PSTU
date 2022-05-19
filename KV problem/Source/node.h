#pragma once

class node : public juce::Component //класс для вершины графа
{
public:
    node() { setSize(50, 50); }

    ~node() {}

    void operator= (node& n)
    {
        this->Index = n.Index;
        this->X = n.X;
        this->Y = n.Y;
        this->isChoised = false;
    }

    void paint (juce::Graphics& g) override
    {
        if(E != dragNode)
            g.setColour(juce::Colours::white);
        if((E == dragNode || E == connect) && isChoised) //если вершина выбрана, то она красная
            g.setColour(juce::Colours::red);

        float thickness = 3;
        juce::Rectangle<float> rect //прямоугольные границы вершины
            (thickness,
             thickness,
             getWidth() - thickness*2,
             getHeight() - thickness*2);

        g.drawEllipse(rect, thickness); //кружок вершины

        g.setFont (24);
        g.drawText(juce::String(Index), rect, //номер вершины
                juce::Justification::centred);
    }

    void setIndex(int n) { Index = n; } //селекторы и модификаторы

    int getMyX() { return X; } void setMyX (int x) { X = x; }
    int getMyY() { return Y; } void setMyY (int y) { Y = y; }

    void resized () override {}

    bool isChoised = false; //выбрана вершина или нет

private:
    int Index, X = 200, Y = 300; //начальные координаты вершины

    void mouseDrag (const juce::MouseEvent& event) override
    {
        if(E == dragNode && isChoised) //если ивент перетаскивания и вершины выбрана, то ее можно перетащить
        {
            beginDragAutoRepeat(1);
            X += event.x - getWidth();
            Y += event.y - getHeight();
        }
    }

    void mouseEnter (const juce::MouseEvent& event) override //когда курсов пересекает границы вершины, она становится выбранной
    {
        if (E == dragNode) //если сейчас ивент перетаскивания
            isChoised = true;
    }

    void mouseExit (const juce::MouseEvent& event) override //когда курсор вне вершины, вершина не выбрана
    {
        if (E == dragNode)
            isChoised = false;
    }

    void mouseDown (const juce::MouseEvent& event) override //если ивент соединения или вывода ответа, выбрать вершину можно щелчком мыши
    {
        if (E == connect || E == getAnswer)
            isChoised = true;
    }
};
