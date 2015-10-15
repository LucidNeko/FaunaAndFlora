#include <map>
#include "comp308.hpp"
#include <string>

class LSystem;
typedef void (*pf)(LSystem*);
using namespace std;
using namespace comp308;

class LSystem{
    map<char,string> rules;
    map<char,pf> insts;
    map<int,string> evald;
    string axiom;
    GLfloat radtheta;
    map<int, int> matlookup;
    void setmat(int);
    void draw_pot();
    GLfloat counter;
    GLfloat original_theta;

public:
    void tick();
    vector<GLfloat> thickness;
    GLfloat theta;
    LSystem(string, string, GLfloat, string);
    void draw(int);
    void drawpoly(string);
    void rx(vec3&, GLfloat);
    void ry(vec3&, GLfloat);
    void rz(vec3&, GLfloat);
};

//LSystem *tree = new LSystem("A:[&FL!A]/////`[&FL!A]///////`[&FL!A] "
//                            "F:S/////F "
//                            "S:FL "
//                            "L:[```^^{-f+f+f-|-f+f+f}]",
//                       "A",22.5,"0:51:25:0 14:0:51:0");
//
//LSystem *tree2 = new LSystem("P:I+[P+F]--//[--L]I[++L]-[PF]++PF "
//                            "I:FS[//&&L][//^^L]FS "
//                            "S:SFS "
//                            "L:[`{+f-ff-f+|+f-ff-f}] "
//                            "F:[&&&p`/W////W////W////W////W] "
//                            "p:FF "
//                            "W:[`^F][{&&&&-f+f|-f+f}]",
//                    "P",18.0,"0:51:25:0 16:0:102:0 13:255:255:102");
