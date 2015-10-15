#include <map>
#include "comp308.hpp"
#include <string>
#include "lsystemtree.hpp"

#define PI 3.14159265

using namespace std;

void forwarddraw(LSystem*);

GLfloat mats[][3][4] = {{{0.1,0.18725,0.1745,1.0},{0.396,0.74151,0.69102,1.0},{0.297254,0.30829,0.306678,1.0}},//0: turquoise
                        {{0.0,0.0,0.0,1.0},{0.55,0.55,0.55,1.0},{0.7,0.7,0.7,1.0}},//1: White plastic
                        {{0.2125,0.1275,0.054,1.0},{0.714,0.4284,0.18144,1.0},{0.393548,0.271906,0.166721,1.0}},//2: bronze
                        {{0.135,0.2225,0.1575,0.95},{0.54,0.89,0.63,0.95},{0.316228,0.316228,0.316228,0.95}},//3: Jade
                        {{0.05,0.05,0.0,1.0},{0.5,0.5,0.4,1.0},{0.7,0.7,0.04,1.0}},//4: rubber yellow
                        {{0.05375,0.05,0.06625,1.0},{0.18275,0.17,0.22525,1.0},{0.0,0.0,0.0,1.0}}};//5: obsidian
                        //{{},{},{}},

void forward(LSystem *ls){
    glTranslatef(0.0,0.0,0.1);
}

//void forwarddraw(LSystem *ls){
//    glBegin(GL_LINES);
//    glVertex3f(0.0,0.0,0.0);
//    glVertex3f(0.0,0.0,0.1);
//    glEnd();
//    forward(ls);
//}

void forwarddraw(LSystem *ls){
    GLUquadricObj *q = gluNewQuadric();
    gluCylinder(q,ls->thickness.back(),ls->thickness.back(),0.1,3,3);
    forward(ls);
}

void inccolor(LSystem *ls){}
void decdiam(LSystem *ls){
    ls->thickness.back()*=0.8;
    glLineWidth(ls->thickness.back());
}

void leftbrac(LSystem *ls){
    glPushMatrix();
    ls->thickness.push_back(ls->thickness.back());
}
void rightbrac(LSystem *ls){
    glPopMatrix();
    ls->thickness.pop_back();
    glLineWidth(ls->thickness.back());
}

void rotxpos(LSystem *ls){glRotatef(ls->theta,1.0,0.0,0.0);}
void rotxneg(LSystem *ls){glRotatef(-ls->theta,1.0,0.0,0.0);}
void rotypos(LSystem *ls){glRotatef(ls->theta,0.0,1.0,0.0);}
void rotyneg(LSystem *ls){glRotatef(-ls->theta,0.0,1.0,0.0);}
void rotzpos(LSystem *ls){glRotatef(ls->theta,0.0,0.0,1.0);}
void rotzneg(LSystem *ls){glRotatef(-ls->theta,0.0,0.0,1.0);}
void flip(LSystem *ls){glRotatef(180.0,0.0,1.0,0.0);}

LSystem::LSystem(string rulesstr, string a, GLfloat th, string colours):theta(th),axiom(a){
    insts['[']=leftbrac;
    insts[']']=rightbrac;
    insts['F']=forwarddraw;
    insts['f']=forward;
    insts['+']=rotypos;
    insts['-']=rotyneg;
    insts['&']=rotxpos;
    insts['^']=rotxneg;
    insts['\\']=rotzpos;
    insts['/']=rotzneg;
    insts['|']=flip;
    insts['`']=inccolor;
    insts['!']=decdiam;

    original_theta = th;
    radtheta = (th*PI)/180.0;

    stringstream rulestream(rulesstr);
    size_t spliti;
    string rule;
    while(rulestream >> rule){
        spliti = rule.find(':');
        string sub = rule.substr(spliti+1,rule.length()-1);
        rules[rule[spliti-1]] = sub;
        cout << "\nsetting " << rule[spliti-1] << " to " << rule.substr(spliti+1,rule.length()-1) << "\n";
    }

    stringstream colourstream(colours);
    string colour;
    int plen,coli;
    while(colourstream >> colour){
        sscanf(colour.c_str(),"%i:%i",&plen,&coli);
        matlookup[plen] = coli;
    }
}

void LSystem::tick(){
    counter += 0.02;
    theta = original_theta + sin(counter);
    radtheta = (theta*PI)/180.0;
}

void LSystem::draw(int iteration){
    draw_pot();

    thickness.clear();
    thickness.push_back(0.013);
    string s;
    map<int,string>::iterator it = evald.find(iteration);
    if(it == evald.end()){
        s = axiom;
        map<char,string>::iterator it1;
        string repl;
        for(int iter=0;iter<iteration;iter++){
            //cout << iter << ": " << s << "\n";
            for(uint i=0; i<s.length(); i++){
                it1 = rules.find(s[i]);
                if(it1 != rules.end()){
                    repl = it1->second;
                    s.replace(i,1,repl);
                    i+=repl.length()-1;
                    //cout << "   " << s[i] << " to " << repl << "  :::  " << s << "\n";
                }
            }
            //cout << iter << ": " << s << "\n";
        }
        //cout << "\n\n";
        evald[iteration]=s;
    }
    else{
        s = string(it->second);
    }

    setmat(matlookup[0]);
    string polystr;
    bool poly = false;
    map<char,pf>::iterator it2;
    pf f;
    glPushMatrix();
    for(uint i=0; i<s.length(); i++){
        if(s[i] == '{') poly=true;
        else if(s[i] == '}'){
            setmat(matlookup[polystr.length()]);
            poly=false;
            drawpoly(polystr);
            polystr.clear();
            setmat(matlookup[0]);
        }
        else if(poly) polystr += s[i];
        else{
            it2 = insts.find(s[i]);
            if(it2 != insts.end()){
                f = it2->second;
                (*(f))(this);
            }
        }
    }
    glPopMatrix();
}

void LSystem::draw_pot(){
    GLUquadricObj *q = gluNewQuadric();
    gluCylinder(q,0.5,0.8,1.0,30,30);
    gluDisk(q,0.0,0.5,30,30);
    glTranslatef(0.0,0.0,0.9);
    setmat(5);
    gluDisk(q,0.0,0.7,10,10);
}

void LSystem::setmat(int mati){
    GLfloat shiny = 0.25;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mats[mati][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mats[mati][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mats[mati][2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shiny);
}

void LSystem::drawpoly(string polystr){
    vec3 v = vec3(0.0,0.0,0.0);
    vec3 dir = vec3(0.0,0.0,0.1);
    //cout << polystr;
    GLfloat flip = 1.0;

    glBegin(GL_POLYGON);
    for(uint i=0; i<polystr.length(); i++){
        char c = polystr[i];
        if(c=='f'){
            //cout << "drawing " << v.x << ", " << v.y << ", " << v.z <<endl;
            glVertex3f(v.x,v.y,v.z);
            v+=dir;
        }
        else if(c=='+') ry(dir,radtheta);
        else if(c=='-') ry(dir,-radtheta);
        else if(c=='\\') rz(dir,radtheta);
        else if(c=='/') rz(dir,-radtheta);
        else if(c=='&') rx(dir,radtheta);
        else if(c=='^') rx(dir,-radtheta);
        else if(c=='|') ry(dir,PI);
        //cout << "after " << c << " dir is " << dir.x << "," << dir.y << "," << dir.z << endl;
    }
    glEnd();

    GLfloat revrot = atan((dir.z/dir.x)) * 180/PI;
    glRotatef(90.0+revrot,0.0,1.0,0.0);
    //cout << "\n\n";
}

void LSystem::rx(vec3 &v, GLfloat th){
    GLfloat newx = v.x*cos(th)-v.y*sin(th);
    GLfloat newy = v.x*sin(th)+v.y*cos(th);
    v.x = newx;
    v.y = newy;
}

void LSystem::ry(vec3 &v, GLfloat th){
    GLfloat newx = v.x*cos(th)+v.z*sin(th);
    GLfloat newz = v.z*cos(th)-v.x*sin(th);
    v.x = newx;
    v.z = newz;
}

void LSystem::rz(vec3 &v, GLfloat th){
    GLfloat newy = v.y*cos(th)-v.z*sin(th);
    GLfloat newz = v.y*sin(th)+v.z*cos(th);
    v.y = newy;
    v.z = newz;
}
