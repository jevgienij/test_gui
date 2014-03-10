//#####################################################################
//       (C) ETC-PZL Aerospace Industries Ltd, Warsaw, Poland         #
//#####################################################################
// This software is copyrighted by                                    #
// ETC-PZL Aerospace Industries Ltd,                                  #
// Al. Krakowska 110/114 , 02-972 Warszawa PL.                        #
//#####################################################################
// File: ai_socket.h                                                  #
// Author: Piotr Krosnowski, Wojciech Giemza, Pawel Blaszczyk         #
// contents: UDP transmission library                                 #
//#####################################################################

#ifndef _AI_SOCKET_H_
#define _AI_SOCKET_H_

#undef __AI_SOCKET_OS__

#include <iostream>

#if ( (defined _LINUX_) || (defined _QNX6_) )
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <stdlib.h>
#  include <string.h>
#  define __AI_SOCKET_OS__
#endif

#ifdef _QNX
   typedef int socklen_t;
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <ioctl.h>
#  include <unix.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <stdlib.h>
#  include <string.h>
#  define __AI_SOCKET_OS__
#endif

#ifdef _VX_WORKS_
   typedef int socklen_t;
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <stdlib.h>
#  include <string.h>
#  include <hostLib.h>
#  include <ioLib.h>
#  include <sockLib.h>
#  define __AI_SOCKET_OS__
#endif

#ifdef WIN32
   typedef int socklen_t;
#  if defined _CYGWIN_
#    include <ws2tcpip.h>
#  endif
#  include <windows.h>
#  define __AI_SOCKET_OS__
#endif

#ifndef __AI_SOCKET_OS__
  To jest celowy blad !!!
  /*
  Nalezy zdefiniowac system operacyjny
  Obslugiwane systemy:
   _LINUX_     :  Linux
   _QNX6_      :  Qnx 6.xx
   _QNX        :  Qnx 4.xx
   _VX_WORKS_  :  VxWorks
   WIN32       :  MSWindows
                  (dla srodowiska CygWin nalezy dodac -D_CYGWIN_)
  */
#endif
#undef __AI_SOCKET_OS__

//####################################################################
/// Klasa AI_UDP_Socket.
///  Tworzy socket UDP i konfiguruje dedykowany adres odbiorcy.
class AI_UDP_Socket
{
  public:
    /// konstruktor klasy AI_UDP_Socket.
    AI_UDP_Socket ( void );
    
    /// destruktor klasy AI_UDP_Socket.
    ~AI_UDP_Socket ();

//####################################################################
    /**
    Metoda czytajaca plik konfiguracyjny i inicjujaca socket.
    @param filename : wskaznik do nazwy pliku konfiguracyjnego
    @param section : wskaznik do nazwy sekcji z danymi socketu
                     (nazwa musi byc z nawiasami kwadratowymi)
    @return
      -1 - blad - wejscie i wyjscie nie zostalo zainicjowane
           lub blad wewnetrzny,
           informacja o typie bledu wysylana jest na stderr
       0 - O.K. - zainicjowano wejscie i wyjscie, 
       1 - O.K. - zainicjowano tylko wyjscie, nie ma wejscia, 
       2 - O.K. - zainicjowano tylko wejscie, nie ma wyjscia; 
      Jesli w pliku konfiguracyjnym wystapi zapis "H auto" to
      uznaje sie, ze wyjscie jest zainicjowane. Jest ono jednak
      nieaktywne do momentu otrzymania pierwszego pakietu.
    */
    int read_config ( const char *filename, const char *section );

//####################################################################
    /**
    Metoda okresla czy socket jest blokujacy czy nieblokujacy.
    @param iBlocking :  1 - socket blokujacy, 0 - socket nieblokujacy
    @return 0 - sukces, -1 - blad
    */
    int block ( int iBlocking );
    
//####################################################################
    /**
    Metoda ustawia opcje IP_MULTICAST_LOOP.
    @param ucLoop :  0, 1 - wartosc IP_MULTICAST_LOOP
    @return 0 - sukces, -1 - blad
    */
    int set_multicast_loop ( unsigned char ucLoop );
    
//####################################################################
    /**
    Metoda ustawia opcje IP_MULTICAST_TTL.
    @param ucTTL :  0, 1 - wartosc IP_MULTICAST_TTL
    @return 0 - sukces, -1 - blad
    */
    int set_multicast_ttl ( unsigned char ucTTL );
    
//####################################################################
    /**
    Metoda wymusza ponowna autodetekcje hosta zdalnego.
    Metoda dziala tylko przy aktywnej opcji "HostOut - auto"
    */
    void detect_remote_host ( void );
    
//####################################################################
    /**
    Metoda czytajaca z socketu bufor o okreslonej dlugosci.
    @param pBuf :  wskaznik do bufora, do ktorego beda kopiowane dane z socketu
    @param iSize :  liczba bajtow do odczytania
    @return >0 - liczba odebranych bajtow, -1 - blad
    */
    int recvfrom ( void* pBuf, size_t iSize );

//####################################################################
    /**
    Metoda wysylajaca bufor o okreslonej dlugosci do zdefiniowanego odbiorcy.
    @param pBuf :  wskaznik do bufora, z ktorego beda kopiowane dane do wyslania
    @param iSize :  liczba bajtow do wyslania
    @return >0 - liczba wyslanych bajtow, -1 - blad
    */
    int sendto ( void* pBuf, size_t iSize );
    /**
    Metoda wysylajaca bufor o okreslonej dlugosci do dowolnego odbiorcy.
    @param as :  wskaznik do struktury zawierajacej adres odbiorcy
    @param pBuf :  wskaznik do bufora, z ktorego beda kopiowane dane do wyslania
    @param iSize :  liczba bajtow do wyslania
    @return >0 - liczba wyslanych bajtow, -1 - blad
    */
    int sendto ( sockaddr_in* as, void* pBuf, size_t iSize );
    

//####################################################################
    // Akcesory
    
    /** Akcesor identyfikatora.
    @return Identyfikator socketu
    */
    int id    ( void ) const { return idSocket; }
    /** Akcesor typu protokolu.
    @return Identyfikator typu protokolu
    */
    int type  ( void ) const { return idType; }
    /** Akcesor rodziny protokolow.
    @return Identyfikator rodziny protokolow
    */
    int family( void ) const { return idFamily; }
    /** Akcesor do parametru iAutoRemoteHost.
    @return Wartosc iAutoRemoteHost 0, 1 (1 - opcja aktywna)
    */
    int auto_remote_host( void ) const { return iAutoRemoteHost; }


    /** Akcesor nazwy lub numeru IP hosta zdalnego.
    @return Wskaznik do lancucha z nazwa lub numerem IP hosta zdalnego
    */
    const char *Host_Out     ( void );
    /** Akcesor numeru poru hosta zdalnego.
    Jesli numer portu > 0 - wyjscie jest aktywne
    @return Numer portu hosta zdalnego
    */
    const long Port_Out      ( void );
    /** Akcesor adresu hosta zdalnego.
    @return Wskaznik do lancucha z adresem hosta zdalnego
    */
    const char *Adress_Out   ( void );
    

    /** Akcesor nazwy lub numeru IP hosta lokalnego.
    @return Wskaznik do lancucha z nazwa lub numerem IP hosta lokalnego
    */
    const char *Host_Local   ( void );
    /** Akcesor numeru poru hosta lokalnego.
    Jesli numer portu > 0 - wejscie jest aktywne
    @return Numer portu hosta lokalnego
    */
    const long Port_Local    ( void );
    /** Akcesor adresu hosta lokalnego.
    @return Wskaznik do lancucha z adresem hosta lokalnego
    */
    const char *Adress_Local ( void );
    
    /** Akcesor nazwy lub numeru IP hosta zdalnego typu multicast.
    @return Wskaznik do lancucha z nazwy lub numerem IP hosta zdalnego typu multicast
    */
    const char *Host_Multi   ( void );
    /** Akcesor adresu hosta zdalnego typu multicast.
    @return Wskaznik do lancucha z adresem hosta zdalnego typu multicast
    */
    const char *Adress_Multi ( void );

  private:
//####################################################################
    //	Funkcja inicjujaca socket na odpowiednie wartosci
    //	Zwraca identyfikator zainicjowanego socket'a lub
    //  -1 w przypadku bledu
    int init_sock ( void );
    
//####################################################################
    //  Funkcja statyczna zwracajaca strukture typu sockaddr_in
    //  Jako parametry wejsciowe nalezy podac:
    //  adres IP lub nazwe hosta w postaci lancucha znakow,
    //  numer portu oraz nazwe rodziny protokolow dla ktorej
    //  wyznaczany jest adres
    sockaddr_in adress ( const char* pcAdress,
                         int iPort,
                         int iFamily );
    
//####################################################################
    //	Funkcja bindujaca dany socket z adresem IP
    //  Jesli multi = 0 , to funkcja uzywa adresu IP zapisanego
    //  w strukturze "adress_local", jesli nie socket jest laczony
    //  z adresem "INADDR_ANY" i portem zapisanym w port_local
    //  Zwracane wartosci: 0 - sukces, -1 - blad
    int bind ( void );
    
//####################################################################
    //	Funkcja zapisujaca adres internetowy "addr" do lancucha "straddr"
    void printsock ( sockaddr_in *addr,
                     char *straddr );

//####################################################################
    // Identyfikator (deskryptor) gniazda oraz identyfikatory rodziny
    // i typu protokolow danego gniazda
    int idSocket, idFamily, idType;

//####################################################################
    // Nazwa sekcji w pliku konfiguracyjnym, z ktorej czytane sa dane
    // socketu - uzywana do identyfikacji komunikatow z wnetrza
    // klasy AI_UDP_Socket
    char socket_id[100];
    
    // dane socketu lokalnego, zdalnego i adresu dla opcji multicast
    char host_out[100];
    long port_out;
    long port_out_cnf;      // port zdalny z pliku konfiguracyjnego
    sockaddr_in adress_out;
    char sadress_out[100];

    char host_local[100];
    long port_local;
    sockaddr_in adress_local;
    char sadress_local[100];

    char host_multi[100];
    sockaddr_in adress_multi;
    char sadress_multi[100];
    
    // Opcja automatycznej konfiguracji zdalnego hosta
    // Numer IP i numer socketu nadawcy z pierwszego odebranego
    // pakietu kopiowany jest do danych hosta zdalnego
    // 1 - opcja aktywna
    int iAutoRemoteHost;
    // Informacja o pierwszym odebranym pakiecie
    int iFirstPackage;
    
    // rozmiar struktury adresowej socketu
    socklen_t iSockaddrSize;
    
    // dane socket'a z ktorego odebrano pakiet
    sockaddr_in adress_from;
    socklen_t fromsize;
};

//####################################################################
/**
 Przeciazony operator << wpisujacy adres socketu do strumienia
*/
std::ostream & operator << ( std::ostream &outs, sockaddr_in &as );
/**
 Przeciazony operator << wpisujacy dane wejscia i wyjscia socketu do strumienia
*/
std::ostream & operator << ( std::ostream &outs, AI_UDP_Socket &soc );

#endif	// _AI_SOCKET_H_
