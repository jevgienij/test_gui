#include "test_gui.h"
#include <fstream>

test_gui::test_gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

			
    int iCfg = 0;
	
	TransSock = new AI_UDP_Socket();
    if (!TransSock) exit(-1);
    iCfg = TransSock->read_config("./ai_socket_net.cfg", "[xxx_mapgui]");
    if (iCfg == 0)
    {
        TransSock->block(0);
		std::cerr<<" Socket initialized"<<std::endl;
        std::cerr<<" [xxx_mapgui]"<<std::endl; // TODO nazwa?
        std::cerr<<*TransSock<<std::endl;
    }
    else
    {
        std::cerr<<" [xxx_mapgui] not initialized !!!"<<std::endl;// TODO nazwa?
        if (iCfg < 0) std::cerr<<"   Error reading config file"<<std::endl;
        if (iCfg & 1) std::cerr<<"   Local host unknown:  "<<TransSock->Host_Local()<<std::endl;
        if (iCfg & 2) std::cerr<<"   Output host unknown:  "<<TransSock->Host_Out()<<std::endl;
        std::cerr<<std::endl;
        if (TransSock) delete TransSock;
        TransSock = 0;
    }
	

	connect(ui.emitDataButton, SIGNAL(released()),this,SLOT(handleButton()));
}

test_gui::~test_gui()
{

}

void test_gui::handleButton()
{
	XXX_MAPGUI_STRUCT trans_struct;
	trans_struct_size = sizeof(XXX_MAPGUI_STRUCT);
	
	trans_struct.latitude  = ui.spin1_lat->value();
	trans_struct.longitude = ui.spin2_lon->value();
	trans_struct.altitude  = ui.spin3_alt->value();

    if (TransSock) TransSock->sendto(&trans_struct, trans_struct_size);

	// debug log
	/*std::ofstream Cfg("./log.txt");
	Cfg << "# BEGIN #" << std::endl;	
		
	Cfg << "dUavLatitude " <<trans_struct.latitude <<std::endl;	
	Cfg << "dUavLongitude "<<trans_struct.longitude <<std::endl;	
	Cfg << "dUavAltitude  "<<trans_struct.altitude <<std::endl;	
	Cfg << std::endl;
	Cfg << "# END #"    << std::endl;
    Cfg.close();*/
}
