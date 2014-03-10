/*********************************************************************/
/*        (C) ETC-PZL Aerospace Industries Ltd, Warsaw, Poland       */
/*********************************************************************/
/*  This software is copyrighted by                                  */
/*  ETC-PZL Aerospace Industries Ltd,                                */
/*  Al. Krakowska 110/114 , 02-972 Warszawa PL.                      */
/*********************************************************************/
/* File: ai_socket.cpp                                               */
/* Author: Piotr Krosnowski, Wojciech Giemza, Pawel Blaszczyk        */
/* Contents: UDP transmission library                                */
/*********************************************************************/

#include <ai_socket.h>
#include <stdio.h>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////
/////////////////////////////   PUBLIC   //////////////////////////////
///////////////////////////////////////////////////////////////////////

//*********************************************************************
AI_UDP_Socket :: AI_UDP_Socket ( void )
{
   idSocket = 0;
   idFamily = AF_INET;
   idType = SOCK_DGRAM;
   iSockaddrSize = sizeof( sockaddr_in );
   iAutoRemoteHost = 0;
   iFirstPackage = 0;
}


//*********************************************************************
AI_UDP_Socket::~AI_UDP_Socket()
{
   if( idSocket > 0 )
   {
      #ifdef WIN32
       closesocket( idSocket );
       WSACleanup();
      #else
       close( idSocket );
      #endif
   }
}


//*********************************************************************
int AI_UDP_Socket :: read_config ( const char *filename, const char *section )
{
   int ret = -1; // nic nie zostalo zainicjowane
   char hostname[100];
   char localname[100];
   char multiname[100];
   long outport = 0;
   long inport = 0;
   char rozdzial[100];
   char pomnaz[100];
   char typ, odstep;
   int czytaj = 1;
   int jest_out, jest_local, jest_multi;
   int read_file = 0;
   ifstream config;

   if ( idSocket > 0 )
   {
      #ifdef WIN32
       closesocket( idSocket );
      #else
       close( idSocket );
      #endif
   }

   rozdzial[0] = 0;
   hostname[0] = 0;
   localname[0] = 0;
   multiname[0] = 0;
   socket_id[0] = 0;
   strcpy(socket_id, section);
   
   config.open(filename);
   if (config)
   {
      read_file = 1;
      // przegladam plik do czasu znalezienia odpowiedniej sekcji
      do
      {
         config >> rozdzial;
         config.getline( pomnaz, 100);
      } while ( strcmp( socket_id, rozdzial) && config.good() );

      // jesli znalazlem - czytam sekwencyjnie
      if (config.good())
      {
         while (czytaj && config.good())
         {
            config >> typ;
            if (typ == '#')
            {
               // komentarz
               config.getline( pomnaz, 100);
            }
            else
            {
               config.get(odstep);
               if (odstep == ' ')
               {
                  switch (typ)
                  {
                     case 'L': // local host name
                       config >> localname;
                       break;
                     case 'H': // host name
                       config >> hostname;
                       break;
                     case 'O': // output port number
                       config >> outport;
                       break;
                     case 'I': // input port number
                       config >> inport;
                       break;
                     case 'M': // multicast group name
                       config >> multiname;
                       break;
                     default:
                       czytaj = 0;
                       break;
                  }
               }
               else
               {
                  czytaj = 0;
               }
            }
         }
      }
      else
      {
         read_file = 0;
         cerr << " Socket " << socket_id
              << " - ERROR : Can't find section: " << socket_id << endl;
      }
      config.clear();
      config.close();
   }
   else
   {
      read_file = 0;
      cerr << " Socket " << socket_id
           << " - ERROR : Can't open file: " << filename << endl;
   }
   
   if (read_file == 1)
   {
      if (strncmp(hostname, "auto", 4) == 0) iAutoRemoteHost = 1;
      jest_out = ((hostname[0] != 0) && (outport != 0));
      jest_local = (inport != 0);
      jest_multi = (multiname[0] != 0);
      #ifdef _QNX // multicasting nie jest obslugiwany
       jest_multi = 0;
      #endif
      if (jest_out || jest_local) // read O.K.
      {
         if (init_sock() != -1)
         {
            ret = 3; //jest socket ale bez ustalonych portow

            if (iAutoRemoteHost == 1)
            {
               ret -= 2; //socket wyjsciowy OK
               port_out = outport;
               port_out_cnf = outport;
            }
            else
            {
               if (jest_out)
               {
                  strcpy( host_out, hostname);
                  port_out = outport;
                  port_out_cnf = outport;
                  adress_out = adress( host_out, port_out, idFamily );
                  printsock(&adress_out, sadress_out);
                  if (adress_out.sin_addr.s_addr != 0)
                  {
                     ret -= 2; //socket wyjsciowy OK
                  }
                  else
                  {
                     port_out = 0; //socket wyjsciowy nieaktywny
                     adress_out = adress( host_out, port_out, idFamily );
                  }
               }
            }
            if (jest_local)
            {
               if (jest_multi)
               {
                  #ifndef _QNX
                   strcpy( host_local, "INADDR_ANY");
                   port_local = inport;
                   adress_local.sin_family = AF_INET;
                   adress_local.sin_port = htons( (unsigned short) port_local);
                   adress_local.sin_addr.s_addr = htonl(INADDR_ANY);
                   printsock(&adress_local, sadress_local);

                   if (bind() == 0)
                   {
                      strcpy( host_multi, multiname);
                      adress_multi = adress( host_multi, port_local, idFamily );
                      printsock(&adress_multi, sadress_multi);
                      ip_mreq ipmr;
                      memcpy( &ipmr.imr_multiaddr, &adress_multi.sin_addr,
                              sizeof(ipmr.imr_multiaddr) );
                      ipmr.imr_interface.s_addr = htonl(INADDR_ANY);

                      int res = setsockopt( idSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                            (const char*)&ipmr, sizeof(ipmr) );
                      if (res == -1)
                      {
                         host_multi[0] = 0;
                         cerr << " Socket " << socket_id
                              << " - ERROR in function setsockopt()" << endl;
                         //char errstring[16] = "Blad";
                         //perror(errstring);
                         ret = -1;  //internal error
                      }
                      else ret -= 1;  //socket wejsciowy OK
                   }
                   else
                   {
                      ret = -1;  //internal error
                      port_local = 0;
                      cerr << " Socket " << socket_id
                           << " - ERROR in function bind()" << endl;
                   }
                  #endif
               }
               else
               {
                  if (localname[0] == 0) //nie bylo linii oznaczonej L
                     gethostname( localname, 100);
                  strcpy( host_local, localname);
                  port_local = inport;
                  adress_local = adress( host_local, port_local, idFamily );
                  printsock(&adress_local, sadress_local);
                  if (adress_local.sin_addr.s_addr != 0)
                  {
                     // poprawny adres lokalny
                     if (bind() == 0)
                     {
                        ret -= 1;  //socket wejsciowy OK
                     }
                     else
                     {
                        ret = -1;  //internal error
                        port_local = 0;
                        cerr << " Socket " << socket_id
                             << " - ERROR in function bind()" << endl;
                     }
                  }
                  else
                  {
                     port_local = 0; //input socket not active
                     adress_local = adress( host_local, port_local, idFamily );
                  }
               }
            }
         }
         else
         {
            cerr << " Socket " << socket_id
                 << " - ERROR in function init_sock()" << endl;
         }
      }
      else
      {
         cerr << " Socket " << socket_id
              << " - ERROR : Output & Local Socket unknown" << endl;
      }
   }

   return( ret);
}


//*********************************************************************
int AI_UDP_Socket::block ( int iBlocking )
{
   unsigned long ulNoBlocking = !iBlocking;

   #if ( (defined _LINUX_) || (defined _QNX6_) )
    return ioctl( idSocket, FIONBIO, &ulNoBlocking );
   #endif

   #ifdef _QNX
    return ioctl( ( short) idSocket, FIONBIO, &ulNoBlocking );
   #endif

   #ifdef _VX_WORKS_
    return ioctl( idSocket, FIONBIO, (int*) &ulNoBlocking );
   #endif

   #ifdef WIN32
    return ioctlsocket( idSocket, FIONBIO, &ulNoBlocking );
   #endif
}


//*********************************************************************
int AI_UDP_Socket::set_multicast_loop ( unsigned char ucLoop )
{
   #if ( (defined _LINUX_) || (defined _QNX6_) || (defined _VX_WORKS_) )
    u_char loop = ucLoop;
    return setsockopt( idSocket, IPPROTO_IP, IP_MULTICAST_LOOP,
                       &loop, sizeof(loop) );
   #endif
   
   #ifdef _QNX
    (void) ucLoop;
    return -1;
   #endif
   
   #ifdef WIN32
    char loop = (char) ucLoop;
    return setsockopt( idSocket, IPPROTO_IP, IP_MULTICAST_LOOP,
                       &loop, sizeof(loop) );
   #endif
}


//*********************************************************************
int AI_UDP_Socket::set_multicast_ttl ( unsigned char ucTTL )
{
   #if ( (defined _LINUX_) || (defined _QNX6_) || (defined _VX_WORKS_) )
    u_char ttl = ucTTL;
    return setsockopt( idSocket, IPPROTO_IP, IP_MULTICAST_TTL,
                       &ttl, sizeof(ttl) );
   #endif
   
   #ifdef _QNX
    (void) ucTTL;
    return -1;
   #endif
   
   #ifdef WIN32
    char ttl = (char) ucTTL;
    return setsockopt( idSocket, IPPROTO_IP, IP_MULTICAST_TTL,
                       &ttl, sizeof(ttl) );
   #endif
}


//*********************************************************************
void AI_UDP_Socket :: detect_remote_host ( void )
{
   iFirstPackage = 0;
}

//*********************************************************************
int AI_UDP_Socket :: recvfrom( void* pBuf, size_t iSize )
{
   int ret = -1;
   
   if (port_local > 0)
   {
      fromsize = iSockaddrSize;
      #if ( (defined _LINUX_) || (defined _QNX6_) || (defined _VX_WORKS_) )
       ret = ::recvfrom( idSocket, pBuf, iSize, 0,
                         (struct sockaddr*) &adress_from, &fromsize );
      #endif
      
      #ifdef _QNX
       ret = ::recvfrom( idSocket, pBuf, iSize, 0,
                         (struct sockaddr*) &adress_from, (int*) &fromsize );
      #endif
      
      #ifdef WIN32
       ret = ::recvfrom( idSocket, (char*) pBuf, iSize, 0,
                         (struct sockaddr*) &adress_from, &fromsize );
      #endif
   }
   
   if (iAutoRemoteHost == 1)
   {
      if (iFirstPackage == 0 && ret != -1)
      {
         iFirstPackage = 1;
         cerr << " Socket " << socket_id
              << ": Remote Host configured" << endl;
         memcpy(&adress_out, &adress_from, iSockaddrSize);
         
         if (port_out_cnf != 0)
         {
            port_out = port_out_cnf;
            adress_out.sin_port = htons( (unsigned short)(port_out) );
         }
         else
            port_out = ntohs(adress_out.sin_port);
         
         printsock(&adress_out, sadress_out);
         cerr << "   address: " << adress_out << endl;
         sprintf(host_out,"auto");
      }
   }
   
   return( ret);
}


//*********************************************************************
int AI_UDP_Socket :: sendto( sockaddr_in* as, void* pBuf, size_t iSize )
{
   #if ( (defined _LINUX_) || (defined _QNX6_) || (defined _VX_WORKS_) )
    return ::sendto( idSocket, pBuf, iSize, 0,
                     (struct sockaddr*) as, iSockaddrSize );
   #endif

   #ifdef _QNX
    return ::sendto( idSocket, pBuf, iSize, 0,
                     (struct sockaddr*) as, (int)iSockaddrSize );
   #endif

   #ifdef WIN32
    return ::sendto( idSocket, (char*)pBuf, iSize, 0,
                     (struct sockaddr*) as, iSockaddrSize );
   #endif
}


//*********************************************************************
int AI_UDP_Socket :: sendto( void* pBuf, size_t iSize )
{
   int ret = -1;
   
   if (port_out > 0)
      ret = sendto( &adress_out, pBuf, iSize );
   
   return (ret);
}


//*********************************************************************
// Akcesory
const char* AI_UDP_Socket :: Host_Out ( void )
{
   return (host_out);
}


const long AI_UDP_Socket :: Port_Out ( void )
{
   return (port_out);
}


const char* AI_UDP_Socket :: Adress_Out ( void )
{
   return (sadress_out);
}


const char* AI_UDP_Socket :: Host_Local ( void )
{
   return (host_local);
}


const long AI_UDP_Socket :: Port_Local ( void )
{
   return (port_local);
}


const char* AI_UDP_Socket :: Adress_Local ( void )
{
   return (sadress_local);
}


const char* AI_UDP_Socket :: Host_Multi ( void )
{
   return (host_multi);
}


const char* AI_UDP_Socket :: Adress_Multi ( void )
{
   return (sadress_multi);
}



///////////////////////////////////////////////////////////////////////
/////////////////////////////   PRIVATE   /////////////////////////////
///////////////////////////////////////////////////////////////////////

//*********************************************************************
int AI_UDP_Socket :: init_sock ( void )
{
   #ifdef WIN32
    WSADATA wsaData;
    WSAStartup( 0x0101, &wsaData );
   #endif

   idSocket = ::socket( idFamily, idType, 0 );

   memset(host_out, 0, sizeof host_out);
   port_out = 0;
   port_out_cnf = 0;
   memset(&adress_out, 0, iSockaddrSize );
   memset(sadress_out, 0, sizeof sadress_out);

   memset(host_local, 0, sizeof host_local);
   port_local = 0;
   memset(&adress_local, 0, iSockaddrSize );
   memset(sadress_local, 0, sizeof sadress_local);

   memset(host_multi, 0, sizeof host_multi);
   memset(&adress_multi, 0, iSockaddrSize );
   memset(sadress_multi, 0, sizeof sadress_multi);
	
   return idSocket;
}


//*********************************************************************
sockaddr_in AI_UDP_Socket :: adress ( const char* pcAdress,
                                      int iPort,
                                      int iFamily )
{
   char acAdress[256] = "";

   if ( pcAdress )
   {
      strncpy( acAdress, pcAdress, sizeof ( acAdress ) );
   }

   sockaddr_in adres;
   memset( (void*)&adres, 0, iSockaddrSize );

   adres.sin_family = (unsigned char) iFamily;
   adres.sin_port   = htons( (unsigned short) iPort );

   if ( *acAdress )
   {
      int iaddr = inet_addr( acAdress );
      if (iaddr < 0)
      {
        #ifdef _VX_WORKS_
         adres.sin_addr.s_addr = hostGetByName( acAdress );
        #else
         struct hostent * phe = gethostbyname( acAdress );
         if ( phe )
         {
	    size_t minlen = ( phe->h_length < 4 ? phe->h_length : 4 );
            memcpy( (void*) & (adres.sin_addr.s_addr),
		    (void*) phe->h_addr, minlen );
         }
        #endif
      }
      else
      {
         adres.sin_addr.s_addr = iaddr;
      }
   }
   else
   {
      adres.sin_addr.s_addr = 0;
   }

   return adres;
}


//*********************************************************************
int AI_UDP_Socket :: bind ( void )
{
   return ::bind( idSocket, (struct sockaddr*) &adress_local, iSockaddrSize );
}


//*********************************************************************
void AI_UDP_Socket :: printsock ( sockaddr_in *addr,
                                  char *straddr )
{
   unsigned char *puc = (unsigned char *)&(addr->sin_addr.s_addr);

   sprintf ( straddr, "%-d.%-d.%-d.%-d:%-d",
             (int)puc[0],
             (int)puc[1],
             (int)puc[2],
             (int)puc[3],
             ntohs(addr->sin_port) );
}



///////////////////////////////////////////////////////////////////////
////////////////////////////   OPERATORY   ////////////////////////////
///////////////////////////////////////////////////////////////////////

//*********************************************************************
ostream & operator << ( ostream &outs, sockaddr_in &as )
{
   unsigned char *puc = (unsigned char *)&(as.sin_addr.s_addr);
   
   outs << (int)puc[0] << "." << (int)puc[1] << "."
        << (int)puc[2] <<"." << (int)puc[3]
        << ":" << ntohs(as.sin_port);

   return outs;
}

//*********************************************************************
ostream & operator << ( ostream &outs, AI_UDP_Socket &soc )
{
   outs << " Local (" << soc.Host_Local() << ":" << soc.Port_Local()
        << ") address: " << soc.Adress_Local() << endl;

   char *hm = (char *)soc.Host_Multi();
   if (hm[0] != 0)
   {
      outs << " Multicast address: " << soc.Adress_Multi() <<endl;
   }

   if (soc.auto_remote_host() == 1)
      outs << " Remote (Automatic) address: " << soc.Adress_Out() << endl;
   else
      outs << " Remote (" << soc.Host_Out() << ":" << soc.Port_Out()
           << ") address: " << soc.Adress_Out() << endl;

   return outs;
}
