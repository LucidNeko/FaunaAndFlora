#include <map>
#include "comp308.hpp"
#include <string>
#include "lsystemtree.hpp"

#define PI 3.14159265

using namespace std;

void forwarddraw(LSystem*);

void forward(LSystem *ls){
    glTranslatef(0.0,0.0,0.1);
}

void forwarddraw(LSystem *ls){
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.1);
    glEnd();
    forward(ls);
}

//void forwarddraw(LSystem *ls){
//    GLUquadricObj *q = gluNewQuadric();
//    gluCylinder(q,thickness,thickness,0.1,10,10);
//    forward(ls);
//}

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
    string numverts,rs,gs,bs;
    while(colourstream >> colour){
        stringstream onecolstream(colour);
        getline(onecolstream,numverts,':');
        getline(onecolstream,rs,':');
        getline(onecolstream,gs,':');
        getline(onecolstream,bs,':');
        vec3 c = vec3(atoi(rs.c_str())/255.0, atoi(gs.c_str())/255.0, atoi(bs.c_str())/255.0);
        cout << atoi(numverts.c_str());
        colourvecs[atoi(numverts.c_str())] = c;
    }
}

void LSystem::draw(int iteration){
    thickness.clear();
    thickness.push_back(5.0);
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

    vec3 c = colourvecs[0];
    glColor3f(c.x,c.y,c.z);
    string polystr;
    bool poly = false;
    map<char,pf>::iterator it2;
    pf f;
    glPushMatrix();
    for(uint i=0; i<s.length(); i++){
        if(s[i] == '{') poly=true;
        else if(s[i] == '}'){
            c = colourvecs[polystr.length()];
            glColor3f(c.x,c.y,c.z);
            poly=false;
            drawpoly(polystr);
            polystr.clear();
            c = colourvecs[0];
            glColor3f(c.x,c.y,c.z);
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
