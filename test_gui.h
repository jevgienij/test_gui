#ifndef TEST_GUI_H
#define TEST_GUI_H

#include <QtWidgets/QMainWindow>
#include "ui_test_gui.h"
#include <ai_socket.h>
#pragma comment(lib, "Ws2_32.lib") // needed by ai_socket to link under Visual Studio

class test_gui : public QMainWindow
{
	Q_OBJECT

public:
	test_gui(QWidget *parent = 0);
	~test_gui();
	// transmission sockets
	AI_UDP_Socket *TransSock;
	size_t trans_struct_size;
	

private slots:
    void handleButton();

private:
	Ui::test_guiClass ui;
};

typedef struct
{
    double         latitude;
    double         longitude;
    double         altitude;
} XXX_MAPGUI_STRUCT;

#endif // TEST_GUI_H
