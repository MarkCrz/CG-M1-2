
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
using namespace std;

//globals

unsigned int obj;
vector<vector<float>> vertices;
vector<vector<int>> faces;
float rot_obj;
float scale[3] = { 1.0, 1.0, 1.0 };
float trans_obj[3] = { 1, 1, -50 };
bool luzDifusa = false;
bool luzEspecular = false;
bool luzAmbiente = false;


void loadObj(string fname)
{
    int read;
    float x, y, z;
    ifstream arquivo(fname);
    if (!arquivo.is_open()) {
        cout << "arquivo nao encontrado";
        exit(1);
    }
    else {
        string tipo;
        while (arquivo >> tipo)
        {

            if (tipo == "v")
            {
                vector<float> vertice;
                float x, y, z;
                arquivo >> x >> y >> z;
                vertice.push_back(x);
                vertice.push_back(y);
                vertice.push_back(z);
                vertices.push_back(vertice);
            }

            if (tipo == "f")
            {
                vector<int> face;
                string x, y, z;
                arquivo >> x >> y >> z;
                int fp = stoi(x.substr(0, x.find("/"))) - 1;
                int fs = stoi(y.substr(0, y.find("/"))) - 1;
                int ft = stoi(z.substr(0, z.find("/"))) - 1;
                face.push_back(fp);
                face.push_back(fs);
                face.push_back(ft);
                faces.push_back(face);
            }
        }
    }




    obj = glGenLists(1);
    glPointSize(2.0);
    glNewList(obj, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_LINES);
        glNormal3f(0.0, 0.0, 1.0);
        const GLfloat kqDelta = .01;
        for (int i = 0; i < faces.size(); i++)
        {
            vector<int> face = faces[i];

            glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
            glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);

            glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);
            glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);

            glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);
            glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);

        }
        glEnd();

    }
    glPopMatrix();
    glEndList();
    arquivo.close();

}


void reshape(int w, int h)
{
    glViewport(1, 1, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    if (luzDifusa) {
        GLfloat luz_difusa[] = { 0.8, 0.8, 0.8, 1.0 };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
        GLfloat posicao_luz[] = { 10,0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    }
    if (luzEspecular) {
        GLfloat luz_especular[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
        GLfloat posicao_luzEsp[] = { 10,0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luzEsp);
    }
    //GLfloat luz_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
    //glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);

    //determina a posiзгo da luz

    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}
void drawObj()
{
    glPushMatrix();
    glTranslatef(trans_obj[0], trans_obj[1], trans_obj[2]);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(scale[0], scale[1], scale[2]);
    glRotatef(rot_obj, 2, 1, 0);
    glCallList(obj);
    glPopMatrix();
}
void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    GLfloat cor_verde[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat cor_vermelha[] = { 1.0, 0.0, 0.0, 1.0 };
    if (luzDifusa) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cor_verde);
    }
    //glMaterialfv(GL_FRONT, GL_SPECULAR, cor_branco);
    //glMaterialf(GL_FRONT, GL_SHININESS, 100);
    drawObj();
    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void escalar(char tipo) {
    if (tipo == '+') {
        cout << scale[0];
        scale[0] = scale[0] + 0.6;
        scale[1] = scale[1] + 0.6;
        scale[2] = scale[2] + 0.6;
    }
    else {
        scale[0] = scale[0] - 0.6;
        scale[1] = scale[1] - 0.6;
        scale[2] = scale[2] - 0.6;
    }
}

void rodar() {
    rot_obj = rot_obj + 0.6;
    if (rot_obj > 360) rot_obj = rot_obj - 360;
}

void translate() {
    if (trans_obj[0] > 32.8 && trans_obj[1] > 32.8 && trans_obj[2] > -29) {
        trans_obj[0] = -94.3999;
        trans_obj[1] = -94.3999;
        trans_obj[2] = -145.4;
    }
    
    trans_obj[0] = trans_obj[0] + 0.6;
    trans_obj[1] = trans_obj[1] + 0.6;
    trans_obj[2] = trans_obj[2] + 0.6;
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 43:
        escalar('+');
        break;
    case 45:
        escalar('-');
        break;
    case 102:
        rodar();
        break;
    case 103:
        translate();
        break;
    case 104:
        cout << luzDifusa;
        luzDifusa = !luzDifusa;
        break;
    case 105:
        luzEspecular = !luzEspecular;
        break;
    case 106:
        luzAmbiente = !luzAmbiente;
        break;
    }

}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Carregar OBJ");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, timer, 0);
    loadObj("C:\\Users\\marco\\source\\repos\\ComputacaoGrafica\\RubixCube.obj");
    glutMainLoop();
    return 0;
}