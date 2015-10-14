#ifndef BUTTON_H
#define BUTTON_H

class Button{
    public:
        Button(void);
        void setPos(float x, float y);
        void setDimensions(float width, float height);
        void setButtonId(int buttonId);
        void setButtonPointerId(int buttonPointerId);
        void setStoredValues(int value1, int value2, int value3);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        float posX, posY;
        float width, height;

        int buttonId, buttonPointerId;

        int storedValue1, storedValue2, storedValue3;

        bool selected, clicked;

    protected:

        bool active;

    private:

};

#endif // BUTTON_H
