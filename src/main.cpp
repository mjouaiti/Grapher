

#include "../../nanogui/include/nanogui/nanogui.h"

using namespace nanogui;

std::vector<std::string> variables {
    "v_e", "y_e", "ss_e", "v_f", "y_f", "ss_f", "Af_e", "Af_f", "Eps_e", "Eps_f"};

enum test_enum {
    v_e, y_e, ss_e, v_f, y_f, ss_f, Af_e, Af_f, Eps_e, Eps_f
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = ss_e;
Color colval(0.5f, 0.5f, 0.7f, 1.f);


GLint WIDTH = 1024, HEIGHT = 768;

int main(int argv, char** argc)
{
    nanogui::init();
    
    Screen *screen = nullptr;
    screen = new Screen(Vector2i(WIDTH + 20, HEIGHT + 20), "NanoGUI test");
    
    bool enabled = true;
    FormHelper *gui = new FormHelper(screen);
    ref<Window> window = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
    
    gui->addGroup("CPG1");
    for(std::string str: variables)
        gui->addVariable(str, bvar);
    
    gui->addGroup("CPG2");
    for(std::string str: variables)
        gui->addVariable(str, bvar);
    
    gui->addGroup("Render");
    gui->addButton("Update", []() { std::cout << "Button pressed." << std::endl; });
    
    screen->setVisible(true);
    screen->performLayout();
    window->center();
    
    nanogui::mainloop();
    
    nanogui::shutdown();
    return 0;
}
