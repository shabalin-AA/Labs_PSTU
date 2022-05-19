#pragma once

#include <JuceHeader.h>
#include <cmath>

int e = -1; //event


struct creditInput //структура для передачи данных о кредите в функцию
{
    double sum, p_c, months; //сумма кредита, процент по кредиту, кол-во месяцев

    creditInput() {}

    void set(double s, double p, double m)
    {
        sum = s;
        p_c = p / 100 / 12;
        months = m;
    }
};

struct creditOutput
{
    long long int overpay, credit; //сумма переплаты по кредиту, сумма кредита с процентами
    int* monthlyPay; //массив с суммами ежемесячных платежей
};


class pays : public juce::Component,            //класс "платежи" - для отображения
                    juce::TextButton::Listener, //ежемесячных платежей
                    juce::Slider::Listener
{
public:
    pays()
    {
        addAndMakeVisible(hideMonthPay); //показать кнопку "скрыть платежи"
        hideMonthPay.addListener(this);

        addAndMakeVisible(s); //показать слайдер окна
        s.addListener(this); //установка некоторых параметров слайдера
        s.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        s.setValue(10);
        s.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

        setSize(getParentWidth(), getParentHeight()); //размер окна устанавливается
                                                    //по размеру основного окна
    }

    void set(creditInput& in, creditOutput& out)
    {
        ci = in; co = out; //установка входных и выходных данных по кредиту
    }

    ~pays() override {}

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        //все заполняется серым цветом
        g.setColour(juce::Colours::white); //цвет всего остального - белый

        if (e == -1)
        {
            repaint();
            mp(g, 0, 30 + ci.months * 24 * (s.getValue() / s.getMaximum() - 1), 50, 24);
        }

        if (e == 3)
        {
            repaint();
            mp(g, 0, 30 + ci.months * 24 * (s.getValue() / s.getMaximum() - 1), 50, 24);
        }

        g.setColour(juce::Colours::antiquewhite);
        g.drawRect(0, 0, 220, 24, 2);
        g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.fillRect(2, 2, 216, 20);

        g.setColour(juce::Colours::white);
        g.drawText("Pay",
            -2, 0, 54, 24,
            juce::Justification::centred, true);

        g.drawText("%",
            -5 + 54, 0, 54, 24,
            juce::Justification::centred, true);

        g.drawText("body",
            -10 + 54*2, 0, 54, 24,
            juce::Justification::centred, true);

        g.drawText("remains",
            -5 + 54*3, 0, 54, 24,
            juce::Justification::centred, true);

    }

    void mp(juce::Graphics& g, int x, int y, int w, int h) //вывод ежемесячных платежей
    {
        int remains = ci.sum;
        for (int i = 0, mpSum = co.monthlyPay[0];
             i < ci.months && remains > 0; i++)
        {
            g.drawText(juce::String(
                std::to_string((int)(ci.p_c * remains))),
                x + w, y, w, h,
                juce::Justification::centred, true);

            remains -= co.monthlyPay[i] - ci.p_c * remains;
            if (remains < 0)
                remains = 0;

            g.drawText(juce::String(std::to_string(
                co.monthlyPay[i])),
                x, y, w, h,
                juce::Justification::centred, true);

            g.drawText(juce::String(
                std::to_string((int)(co.monthlyPay[i] - ci.p_c * remains))),
                x + w*2, y, w, h,
                juce::Justification::centred, true);

            g.drawText(juce::String(
                std::to_string((int)(remains))),
                x+w*3, y, w + 20, h,
                juce::Justification::centred, true);

            mpSum += co.monthlyPay[i];
            y += h;
        }
    }


    void resized() override
    {
        hideMonthPay.setBounds(220, 214, 180, 34);
        s.setBounds(400, 0, 20, 270);
    }

    juce::TextButton hideMonthPay{"Hide monthly payments"};
    virtual void buttonClicked(juce::Button* b) override
    {
        if (b == &hideMonthPay)
            e = 2;
    }

    virtual void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &s) //если изменяется значение слайдера,
            e = 3;        //меняется положение компонентов окна
    }

    juce::Slider s; //слайдер окна

private:
    creditInput ci; creditOutput co; //экземпляры классов входа и выхода по кредиту

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(pays)
};




class MainComponent  : public juce::Component, juce::TextButton::Listener
{
public:
    MainComponent()
    {
        //input textboxes
        addAndMakeVisible(sum); sum.setFont(juce::Font(mainFont));
        sum.setText("100000", true);
        addAndMakeVisible(sumLabel); sumLabel.attachToComponent(&sum, false);
        sumLabel.setFont(juce::Font(sryFont));
        sumLabel.setText("Sum of credit", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(p_c); p_c.setFont(juce::Font(24));
        p_c.setText("5.00", true);
        addAndMakeVisible(p_cLabel); p_cLabel.attachToComponent(&p_c, false);
        p_cLabel.setFont(juce::Font(sryFont));
        p_cLabel.setText("% per year", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(time); time.setFont(juce::Font(24));
        time.setText("3", true);
        addAndMakeVisible(timeLabel); timeLabel.attachToComponent(&time, false);
        timeLabel.setFont(juce::Font(sryFont));
        timeLabel.setText("Term", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(yOrM);
        yOrM.addItem("years", 1); yOrM.addItem("months", 2);
        yOrM.setSelectedItemIndex(yOrM.getSelectedId(), true);

        addAndMakeVisible(type);
        type.addItem("Annuity", 1); type.addItem("Differenced", 2);
        type.setSelectedItemIndex(type.getSelectedId(), true);

        addAndMakeVisible(typeLabel); typeLabel.attachToComponent(&type, false);
        typeLabel.setFont(juce::Font(sryFont));
        typeLabel.setText("Credit type", juce::NotificationType::dontSendNotification);


        //output textboxes
        addAndMakeVisible(overpay); overpay.setReadOnly(true); overpay.setFont(juce::Font(mainFont));
        addAndMakeVisible(opLabel); opLabel.attachToComponent(&overpay, false);
        opLabel.setFont(juce::Font(sryFont));
        opLabel.setText("Overpay", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(monthlyPay); monthlyPay.setReadOnly(true); monthlyPay.setFont(juce::Font(mainFont));
        addAndMakeVisible(mpLabel); mpLabel.attachToComponent(&monthlyPay, false);
        mpLabel.setFont(juce::Font(sryFont));
        mpLabel.setText("Per month", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(credit); credit.setReadOnly(true); credit.setFont(juce::Font(mainFont));
        addAndMakeVisible(creditLabel); creditLabel.attachToComponent(&credit, false);
        creditLabel.setFont(juce::Font(sryFont));
        creditLabel.setText("Credit", juce::NotificationType::dontSendNotification);

        addAndMakeVisible(showMonthPay);
        showMonthPay.addListener(this);

        setSize (width, height);
    }

    ~MainComponent() override {}

    virtual void paint (juce::Graphics& g) override
    {
        double months;
        if (yOrM.getSelectedId() == 1) //месяцы или годы
            months = time.getText().getDoubleValue() * 12;
        if (yOrM.getSelectedId() == 2)
            months = time.getText().getDoubleValue();

        ci.set(sum.getText().getDoubleValue(),
            p_c.getText().getDoubleValue(), months);


        if (type.getSelectedId() == 1) //тип кредита
            co = annuityCredit(ci);
        if (type.getSelectedId() == 2)
            co = difCredit(ci);

        if (!(sum.isEmpty() || time.isEmpty() || p_c.isEmpty()))
        {
            overpay.   setText(juce::String(std::to_string(co.overpay)));
            monthlyPay.setText(juce::String(std::to_string(co.monthlyPay[0])));
            credit.    setText(juce::String(std::to_string(co.credit)));
        }

        p.set(ci, co);

        if (e == 1)
        {
            addAndMakeVisible(p); //показать ежемесячные платежи
            e = -1;
        }

        if (e == 2) //скрыть ежемесячные платежи
        {
            removeChildComponent(getIndexOfChildComponent(&p));
            e = -1;
        }
    }

    virtual void resized() override
    {
        //input textboxes
        int x = 20, y = 20, w = 160, h = 34, border = 12;
        int cmBoxSize = 95;

        sum. setBounds(x, y + sryFont, w, h);
        p_c. setBounds(x, y + (sryFont + h + border) + sryFont, w, h);
        time.setBounds(x, y + (sryFont + h + border)*2 + sryFont, w - cmBoxSize, h);

        yOrM.setBounds(x + w - cmBoxSize, y + (sryFont + h + border) * 2 + sryFont, cmBoxSize, h);
        type.setBounds(x, y + (sryFont + h + border) * 3 + sryFont, w, h);


        //output textboxes
        int Border = 40;
        overpay.   setBounds(x + w + Border, y + sryFont, w + 20, h);
        monthlyPay.setBounds(x + w + Border, y + (sryFont + h + border) + sryFont, w + 20, h);
        credit.    setBounds(x + w + Border, y + (sryFont + h + border) * 2 + sryFont, w + 20, h);

        showMonthPay.setBounds(x + w + Border, y + (sryFont + h + border) * 3 + sryFont, w + 20, h);
    }

private:
    int width = 420, height = 270;

    int mainFont = 24, sryFont = 14;

    juce::TextEditor sum;  juce::Label sumLabel; //текстбоксы и подписи к ним
    juce::TextEditor p_c;  juce::Label p_cLabel;
    juce::TextEditor time; juce::Label timeLabel;

    juce::ComboBox yOrM;
    juce::ComboBox type; juce::Label typeLabel;

    juce::TextEditor overpay;    juce::Label opLabel;
    juce::TextEditor monthlyPay; juce::Label mpLabel;
    juce::TextEditor credit;     juce::Label creditLabel;

    juce::TextButton showMonthPay{ "Show monthly payments" };

    virtual void buttonClicked(juce::Button* b) override
    {
        if (b == &showMonthPay)
            e = 1;
    }

    creditInput ci; creditOutput co;
    pays p;

    creditOutput annuityCredit(creditInput ci) //рассчет аннуитетного кредита
    {
        creditOutput co;
        int m = ci.months;
        co.monthlyPay = new int[m];

        for(int i=0; i<ci.months; i++)
            co.monthlyPay[i] = (int)round(ci.sum * ci.p_c * pow(1 + ci.p_c, ci.months) / (pow(1 + ci.p_c, ci.months) - 1));

        co.credit = (int)round(*co.monthlyPay * ci.months);
        co.overpay = (int)round(co.credit - ci.sum);

        return co;
    }

    creditOutput difCredit(creditInput ci) //расчет диффереенцированного кредита
    {
        creditOutput co;
        double mainPart = ci.sum / ci.months;

        double percentSum = 0, credit = ci.sum;
        int m = ci.months;
        co.monthlyPay = new int[m];

        int i = 0;
        while (credit >= 0)
        {
            percentSum += credit * ci.p_c;
            co.monthlyPay[i] = (int)round(credit * ci.p_c + mainPart);
            credit -= mainPart; i++;
        }

        co.credit = (int)round(ci.sum + percentSum);
        co.overpay = (int)round(co.credit - ci.sum);

        return co;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

