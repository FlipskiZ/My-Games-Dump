#ifndef BUTTON_H
#define BUTTON_H


class Button
{
    public:
        Button();
        void setPos(int x, int y);
        void setDimensions(int width, int height);
        void setButtonId(int buttonId);
        void setActive(bool active);
        bool checkActive();
        void draw();
        void update();

        float posX, posY;
        float width, height;

        int buttonId;

        bool selected;
        bool playButtonPress;
        bool playButtonPressHelper;
        bool buttonPressed;

    protected:

        bool active;

    private:
};

#endif // BUTTON_H
