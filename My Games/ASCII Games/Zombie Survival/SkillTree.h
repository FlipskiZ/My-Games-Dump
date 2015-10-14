#ifndef SKILLTREE_H
#define SKILLTREE_H


class SkillTree
{
    public:
        SkillTree();
        void skillScreen();
    protected:
        int skills[9][5][3];
        char skillNames[9][5][20];
    private:
};

#endif // SKILLTREE_H
