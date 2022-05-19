#pragma once

#include <JuceHeader.h>
#include "event.h"
#include "node.h"
#include "matrix.h"

#define max_size 20
#define inf 100
#define pi 3.14159265

class MainComponent  : public juce::Component,
                       public juce::TextButton::Listener
{
public:
    MainComponent()
    {
        addAndMakeVisible(addButton); //кнопки управления
        addButton.addListener(this);

        addAndMakeVisible(dragButton);
        dragButton.addListener(this);

        addAndMakeVisible(clearButton);
        clearButton.addListener(this);

        addAndMakeVisible(connectButton);
        connectButton.addListener(this);

        addAndMakeVisible(taskButton);
        taskButton.addListener(this);

        m.nullGenerate(); //матрица смежности
        addChildComponent(m);
        addAndMakeVisible(m);

        addChildComponent(s);

        setSize (600, 400);
    }

    ~MainComponent() override {}

    void paint (juce::Graphics& g) override
    {
        paintConnectors(g); //отображение путей между вершинами

        g.setColour(juce::Colours::dimgrey);

        g.drawText("Click to refresh", 0, 380, 150, 20,
            juce::Justification::centred);

        if(E == addNode && iterNodes < max_size) //добавление новой вершины
        {
            nodes[iterNodes].setBounds //обозначаем позицию
               (nodes[iterNodes].getMyX(),
                nodes[iterNodes].getMyY(),
                nodes[iterNodes].getWidth(),
                nodes[iterNodes].getHeight());

            nodes[iterNodes].setIndex(iterNodes + 1); //присваивание номера
            addChildComponent(nodes[iterNodes]); //добавление на экран
            addAndMakeVisible(nodes[iterNodes]);
            iterNodes++;
            m.setCurrentSize(m.getCurrentSize()+1); //увеличение матрицы

            E = noEvent; //обновление ивента
            path.setBounds(0, 0, 0, 0); //сокрытие окошка ввода пути
        }

        if(E == dragNode) //перетаскивание узла
        {
            g.setFont (32);
            g.drawText ("Drag any node you want", //пояснение для пользователя
                        0, 0, 600, 400,
                        juce::Justification::centredTop);
            g.setFont (14);

            for (int i=0; i<max_size; i++)
            {
                if (nodes[i].isChoised) //ищем вершину, которую выбрал пользователь
                {
                    nodes[i].setBounds //перерисовываем в новое место
                        (nodes[i].getMyX(),
                         nodes[i].getMyY(),
                         nodes[i].getWidth(),
                         nodes[i].getHeight());
                }
                repaint();
            }
        }

        if(E == clearScreen) //очистка экрана и обнуление переменных
        {
            for (; iterNodes >= 0; iterNodes--) //удаление вершин
            {
                nodes[iterNodes].setMyX(200);
                nodes[iterNodes].setMyY(200);
                removeChildComponent(&nodes[iterNodes]);
            }
            iterNodes++;

            path.setBounds(0, 0, 0, 0); //удаление тексбокса для ввода пути
            m.setCurrentSize(0); //размер матрицы смежности - 0
            m.nullGenerate();
            answerCounter = 0; //обнуление счетчика ответов
            s.nullGenerate();
            repaint();
        }

        if(E == connect) //соединение вершин
        {
            g.setFont (32);
            g.drawText ("Choose two nodes",
                        0, 0, 600, 400,
                        juce::Justification::centredTop);
            g.setFont (14);

            for (int i=0; i<max_size; i++)
            {
                if (nodes[i].isChoised) //ищем выбранную и запоминаем индекс
                {
                    if (x < 0 && y < 0)
                        x = i;
                }
                repaint();
            }

            for (int i = 0; i < max_size; i++)
            {
                if (nodes[i].isChoised) //берем еще одну выбранную
                {
                    if (i != x && y < 0)
                    {
                        y = i;

                        nodes[x].isChoised = false;
                        nodes[y].isChoised = false;
                        E = setConnection; //и соединяем
                    }
                }
                repaint();
            }
        }

        if (E == setConnection) //отображение стрелочки и текстбокса для ввода пути
        {
            setConnector (g, &nodes[x], &nodes[y]);
        }

        if(E == runTask) //решение задачи коммивояжера
        {
            g.setColour(juce::Colours::dimgrey);

            if(m.getCurrentSize() > 0)
            {
                answerCounter = 0; //обнуление счетчика ответов
                s.setCurrentSize(m.getCurrentSize());
                s.Copy(m); //копирование матрицы смежности
                task(&s); //вызов функци, решающей задачу
            }
        }

        if (E == getAnswer) //когда функция решения закончила выполнение, выводится ответ
        {
            bool showText = true;
            for (int i=0; i<max_size; i++)
                showText *= !nodes[i].isChoised; //если нет ни одной выбранной вершины, выводится текст

            if (showText)
            {
                g.setFont (32);
                g.drawText ("Choose node to start", //текст
                            0, 0, 600, 400,
                            juce::Justification::centredTop);
                g.setFont (14);
            }

            if(answerCounter > 0)
            {
                for (int i = 0; i < max_size; i++)
                {
                    if (nodes[i].isChoised)
                    {
                        printAnswer(i + 1, g); //ищем выбранную вершину и с нее выводим найденный путь-ответ
                        x = i;
                    }
                    repaint();
                }
            }
        }
    }

    void setConnector (juce::Graphics& g, node* _begin, node* _end) //добавление пути
    {
        path.setBounds( //отображение текстбокса для ввода
            (nodes[y].getMyX() + nodes[x].getMyX()) / 2,
            (nodes[y].getMyY() + nodes[x].getMyY()) / 2,
            25, 16);

        addAndMakeVisible(path);

        m.setValue(x, y, path.getText().getIntValue()); //добавление значение в матрицу смежности
    }

    void resized() override //положение всех кнопок и матрицы смежности на экране
    {
        auto area = getLocalBounds();
        addButton.    setBounds(area.removeFromLeft(130));
        dragButton.   setBounds(area.removeFromLeft(130));
        connectButton.setBounds(area.removeFromLeft(130));
        clearButton.  setBounds(area.removeFromLeft(130));

        addButton.    setBounds(area.removeFromTop(30));
        dragButton.   setBounds(area.removeFromTop(30));
        connectButton.setBounds(area.removeFromTop(30));
        clearButton.  setBounds(area.removeFromTop(30));
        taskButton.   setBounds(area.removeFromTop(30));

        m.setBounds (380, 200, m.getMyWidth(), m.getMyHeight());
    }

private:

    struct nul { int x, y, fine; }; //структура для запоминания нулевы хэлементов матрицы
                                    //х, у - координаты в матрице, fine - оценка нуля
    node nodes[max_size]; //массив вершин
    juce::TextEditor path; //текстбокс для ввода пути
    int iterNodes = 0, x = -1, y = -1; //счетчики
    matrix m; matrix s; //матрица смежности и ее будущая копия для выполнени задания
    nul answer[max_size]; int answerCounter = 0; //массив с путями, которые пойдут в ответ

    juce::TextButton addButton     {"add node"}; //кнопки
    juce::TextButton dragButton    {"drag node"};
    juce::TextButton clearButton   {"clear"};
    juce::TextButton connectButton {"connect"};
    juce::TextButton taskButton    {"task"};

    void buttonClicked (juce::Button* b) //при нажатии на кнопку меняется событие
    {
        if (b == &addButton)
            E = addNode;

        if(b == &dragButton)
            E = dragNode;

        if(b == &clearButton)
            E = clearScreen;

        if(b == &connectButton)
            E = connect;

        if(b == &taskButton)
            E = runTask;
    }

    void paintConnectors(juce::Graphics& g) //рисование стрелочек
    {
        for (int i=0; i<m.getCurrentSize(); i++)
        {
            for (int j=0; j<m.getCurrentSize(); j++)
            {
                if (m.getValue(i, j) != inf)
                {
                    float beg_x = nodes[i].getMyX() + nodes[i].getWidth() / 2;
                    float beg_y = nodes[i].getMyY() + nodes[i].getHeight() / 2;
                    float end_x = nodes[j].getMyX() + nodes[j].getWidth() / 2;
                    float end_y = nodes[j].getMyY() + nodes[j].getHeight() / 2;

                    float dif_x = beg_x - end_x;
                    float dif_y = beg_y - end_y;
                    float dif = sqrt(dif_x*dif_x + dif_y*dif_y);

                    g.setColour(juce::Colours::grey);

                    juce::Line <float> l;
                    l.setStart(beg_x - nodes[i].getWidth()/2 * dif_x / dif,
                               beg_y - nodes[i].getHeight()/2 * dif_y / dif);
                    l.setEnd(end_x + nodes[j].getWidth()/2 * dif_x / dif,
                             end_y + nodes[j].getHeight()/2 * dif_y / dif);

                    g.drawArrow(l, 1, 6, 15);

                    if (i < j) //одно цифры отображаются сверху
                    {
                        g.setColour(juce::Colours::white);
                        g.drawText (juce::String(m.getValue(i, j)),
                            (nodes[i].getMyX() + nodes[j].getMyX())/2,
                            (nodes[i].getMyY() + nodes[j].getMyY())/2,
                            25, 16,
                            juce::Justification::centred);
                    }

                    if (j < i) //а другие снизу стрелочки
                    {
                        g.setColour(juce::Colours::white);
                        g.drawText (juce::String(m.getValue(i, j)),
                            (nodes[i].getMyX() + nodes[j].getMyX())/2 + nodes[i].getWidth()/2,
                            (nodes[i].getMyY() + nodes[j].getMyY())/2 + nodes[i].getHeight()/2,
                            25, 16,
                            juce::Justification::centred);
                    }
                }

            }
        }
    }

    void mouseDown (const juce::MouseEvent& event) override //при нажатии в любом месте обнуляются некоторые переменнные и экран прерисовывается
    {
        E = noEvent;
        path.setBounds(0, 0, 0, 0);
        x = -1; y = -1;

        for (int i=0; i<max_size; i++)
            nodes[i].isChoised = false;

        repaint();
    }

    void task (matrix* M) //решение задачи коммивояжера
    {
        int q = M->getCurrentSize();

        int lnMin = inf; //редукция строк
        for(int i=0; i < q; i++)
        {
            lnMin = inf;
            for(int j=0; j < q; j++)
                if(M->getValue(i, j) < lnMin)
                    lnMin = M->getValue(i, j);

            for(int j=0; j < q; j++)
                if(M->getValue(i, j) != inf)
                    M->setValue(i, j, M->getValue(i, j) - lnMin);
        }

        int colMin = inf; //редукция столбцов
        for(int i=0; i < q; i++)
        {
            colMin = inf;
            for(int j=0; j < q; j++)
                if(M->getValue(j, i) < colMin)
                    colMin = M->getValue(j, i);

            for(int j=0; j < q; j++)
                if(M->getValue(j, i) != inf)
                    M->setValue(j, i, M->getValue(j, i) - colMin);
        }

        nul nulls[q * q];
        int nullcounter = 0;

        for(int i=0; i<q; i++) //вычисление оценок
            for(int j=0; j<q; j++)
                if(M->getValue(i, j) == 0)
                {
                    nulls[nullcounter].x = i;
                    nulls[nullcounter].y = j;

                    nulls[nullcounter].fine = countFine(nulls[nullcounter], M);
                    nullcounter++;
                }

        nul maxFine; maxFine.fine = 0; //нахождение максимальной оценки
        for(int i=0; i<nullcounter; i++)
            if(nulls[i].fine > maxFine.fine)
            {
                maxFine.x = nulls[i].x;
                maxFine.y = nulls[i].y;
                maxFine.fine = nulls[i].fine;
            }

        for(int i=0; i<q; i++) //удаление строки и столбца максимальной оценки
            for(int j=0; j<q; j++)
                if(i==maxFine.x || j==maxFine.y)
                    M->setValue(i, j, inf);

        M->setValue(maxFine.y, maxFine.x, inf);

        answer[answerCounter] = maxFine; //в ответ пишется ноль с максимальной оценкой
        answerCounter++;

        if(!isEnd(M)) task(M); //если не найден путь, выполняется снова
        else //иначе все нули пишутся в ответ
        {
            for (int i=0; i<q; i++)
                for (int j=0; j<q; j++)
                    if(M->getValue(i, j) == 0)
                    {
                        answer[answerCounter].x = i;
                        answer[answerCounter].y = j;
                        answerCounter++;
                    }
            E = getAnswer;
        }
    }

    void printAnswer(int beginning, juce::Graphics& g) //вывод ответа
    {
        int x = 0, y = 0, w = 20, h = 15;
        int q = answerCounter;
        int sum = 0;

        beginning--;
        nul current;

        for (int i=0; i<q; i++)
            for (int j=0; j<q; j++)
                if(answer[i].x == beginning) //ищем вершину, с которой надо начать
                    current = answer[i];

        sum += m.getValue(current.x, current.y); //плюсуем в сумму

        g.setColour(juce::Colours::grey); //выводятся номера вершин, входящих в ответ
        g.drawText(juce::String(current.x + 1),
            x, y, w, h,
            juce::Justification::centred);

        float beg_x = nodes[current.x].getMyX() + nodes[current.x].getWidth() / 2;
        float beg_y = nodes[current.x].getMyY() + nodes[current.x].getHeight() / 2;
        float end_x = nodes[current.y].getMyX() + nodes[current.y].getWidth() / 2;
        float end_y = nodes[current.y].getMyY() + nodes[current.y].getHeight() / 2;

        float dif_x = beg_x - end_x;
        float dif_y = beg_y - end_y;
        float dif = sqrt(dif_x*dif_x + dif_y*dif_y);

        g.setColour(juce::Colours::red); //рисуются красные стрелочки

        juce::Line <float> l;
        l.setStart(beg_x - nodes[current.x].getWidth()/2 * dif_x / dif,
                   beg_y - nodes[current.x].getHeight()/2 * dif_y / dif);
        l.setEnd(end_x + nodes[current.y].getWidth()/2 * dif_x / dif,
                 end_y + nodes[current.y].getHeight()/2 * dif_y / dif);

        g.drawArrow(l, 1, 6, 15);

        x += w;

        for (int i=0; i<q; i++) //для остальных кроме первой то же самое
            if(answer[i].x == current.y && answer[i].x != beginning)
            {
                float beg_x = nodes[current.x].getMyX() + nodes[current.x].getWidth() / 2;
                float beg_y = nodes[current.x].getMyY() + nodes[current.x].getHeight() / 2;
                float end_x = nodes[current.y].getMyX() + nodes[current.y].getWidth() / 2;
                float end_y = nodes[current.y].getMyY() + nodes[current.y].getHeight() / 2;

                float dif_x = beg_x - end_x;
                float dif_y = beg_y - end_y;
                float dif = sqrt(dif_x*dif_x + dif_y*dif_y);

                g.setColour(juce::Colours::red);

                juce::Line <float> l;
                l.setStart(beg_x - nodes[current.x].getWidth()/2 * dif_x / dif,
                           beg_y - nodes[current.x].getHeight()/2 * dif_y / dif);
                l.setEnd(end_x + nodes[current.y].getWidth()/2 * dif_x / dif,
                         end_y + nodes[current.y].getHeight()/2 * dif_y / dif);

                g.drawArrow(l, 1, 6, 15);

                current = answer[i];
                sum += m.getValue(current.x, current.y);

                g.setColour(juce::Colours::grey);
                g.drawText(juce::String(current.x + 1),
                    x, y, w, h,
                    juce::Justification::centred);

                beg_x = nodes[current.x].getMyX() + nodes[current.x].getWidth() / 2;
                 beg_y = nodes[current.x].getMyY() + nodes[current.x].getHeight() / 2;
                 end_x = nodes[current.y].getMyX() + nodes[current.y].getWidth() / 2;
                 end_y = nodes[current.y].getMyY() + nodes[current.y].getHeight() / 2;

                 dif_x = beg_x - end_x;
                 dif_y = beg_y - end_y;
                 dif = sqrt(dif_x*dif_x + dif_y*dif_y);

                g.setColour(juce::Colours::red);

                l.setStart(beg_x - nodes[current.x].getWidth()/2 * dif_x / dif,
                           beg_y - nodes[current.x].getHeight()/2 * dif_y / dif);
                l.setEnd(end_x + nodes[current.y].getWidth()/2 * dif_x / dif,
                         end_y + nodes[current.y].getHeight()/2 * dif_y / dif);

                g.drawArrow(l, 1, 6, 15);

                x += w;
                i = 0;
            }

        g.setColour(juce::Colours::grey);
        g.drawText(juce::String(current.y + 1),
                    x, y, w, h,
                    juce::Justification::centred);

        x += w;
        g.drawText("Sum = " + juce::String(sum), //вывод суммы
                    x, y, w + 100, h,
                    juce::Justification::centred);
    }

    bool isEnd(matrix* M) //проверка на то, найден путь или нет
    {
        int q = M->getCurrentSize();
        int lnMin = inf, colMin = inf;

        for(int i=0; i<q; i++)
        {
            for(int j=0; j<q; j++)
                if(M->getValue(i, j) < lnMin)
                    lnMin = M->getValue(i, j);

            if(lnMin != 0 && lnMin != inf) return false;
        }

        for(int i=0; i<q; i++)
        {
            for(int j=0; j<q; j++)
                if(M->getValue(j, i) < colMin)
                    colMin = M->getValue(j, i);

            if(colMin != 0 && colMin != inf) return false;
        }

        return true;
    }

    int countFine(nul n, matrix* M) //подсчет оценки для нулевого элеменьа
    {
        int q = M->getCurrentSize();

        int _fine = 0, lnMin = inf, colMin = inf;

        for(int i=0; i<q; i++)
            if(M->getValue(n.x, i) < lnMin && i != n.y)
                lnMin = M->getValue(n.x, i);

        for(int i=0; i<q; i++)
            if(M->getValue(i, n.y) < colMin && i != n.x)
                colMin = M->getValue(i, n.y);

        _fine = lnMin + colMin;
        return _fine;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
