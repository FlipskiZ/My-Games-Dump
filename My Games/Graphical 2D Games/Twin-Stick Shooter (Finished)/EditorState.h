#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "GameState.h"

class EditorState : public GameState{
    public:
        void init();
        void cleanup();

        void pause();
        void resume();

        void update(Engine* engine);
        void draw(Engine* engine);

        static EditorState* instance() {
            return &m_EditorState;
        }

    protected:
        EditorState(){}

    private:
        static EditorState m_EditorState;
        int selectedTile;
        int selectedTileMouseWheel;
};


#endif // EDITORSTATE_H
