#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
using namespace std;
using namespace sf;

/// ############################################################################################################################################## ////
/// =========================                           FUNKCJE PONADCZASOWE                        ============================================== ////
/// ############################################################################################################################################## ////

unsigned long long potega(int wykladnik,int podstawa)
{
    if(wykladnik==0)return 1;
   unsigned long long suma=podstawa;
    for(wykladnik;wykladnik>1;wykladnik--)
    {

        suma=suma*podstawa;
    };
    return suma;
}


unsigned long long stringoint(string ciag)
{
    string pomocnik;
    int pomocnik2=0;
    int wroc=0;
    int dlugosc=ciag.length();


    for(dlugosc;dlugosc>0;dlugosc--)
    {

    pomocnik.insert(0,ciag,0,1);
    ciag.erase(0,1);
    if(pomocnik=="1")pomocnik2=potega(dlugosc-1,10)*1;
    else if(pomocnik=="2")pomocnik2=potega(dlugosc-1,10)*2;
    else if(pomocnik=="3")pomocnik2=potega(dlugosc-1,10)*3;
    else if(pomocnik=="4")pomocnik2=potega(dlugosc-1,10)*4;
    else if(pomocnik=="5")pomocnik2=potega(dlugosc-1,10)*5;
    else if(pomocnik=="6")pomocnik2=potega(dlugosc-1,10)*6;
    else if(pomocnik=="7")pomocnik2=potega(dlugosc-1,10)*7;
    else if(pomocnik=="8")pomocnik2=potega(dlugosc-1,10)*8;
    else if(pomocnik=="9")pomocnik2=potega(dlugosc-1,10)*9;
    else pomocnik2=0;
    wroc=wroc+pomocnik2;

    pomocnik.erase(0,1);
    }



    return wroc;
}
string intostring(int n)
{
     string tmp, ret;
     if(n < 0) {
      ret = "-";
      n = -n;
     }
     do {
      tmp += n % 10 + 48;
      n -= n % 10;
     }
     while(n /= 10);
     for(int i = tmp.size()-1; i >= 0; i--)
      ret += tmp[i];
     return ret;
}
Texture broniowa_prawa,broniowa_lewa,artefaktowa;
Texture obosieczna;
Texture zasilaczowa[4];
Texture dyskowa[8];
Music smierc;
Music strzal_dzwiek;
Texture zbrojowa,nadbroniowa,szyjowa,butna,renczna;
int Szerokosc=1600,Wysokosc=1200;
int level_limit[100];
int nr_wiezy_przerabianej=0;
string tekst_automatu="";
Font arial;


/// ############################################################################################################################################## ////
/// =========================                           DEKLARACJA KLAS                             ============================================== ////
/// ############################################################################################################################################## ////


class guziki_format
{
public:
    Sprite obraz;
    int x,y,w,h;
    string komunikat;


    string sprawdz_najechanie(int xx, int yy, bool & smiec)
    {
        string zwrot="";

        if(xx>=x&&xx<=x+w&&yy>=y&&yy<=y+h)
        {
            obraz.setTextureRect(IntRect(w,0,w,h));
            if(smiec)zwrot=sprawdz_klik();
        }
        else obraz.setTextureRect(IntRect(0,0,w,h));
        return zwrot;

    }
    string sprawdz_klik()
    {

                obraz.setTextureRect(IntRect(2*w,0,w,h));
                return komunikat;                                   ///TRZEBA ZROBIĆ OBSŁUGE KOMUNIKATU
        }


    void ustaw_sie()
    {
        obraz.setPosition(x,y);
        obraz.setTextureRect(IntRect(0,0,w,h));
    }
};

class quest_format
{
public:
   string nazwa;
   string typ_zadania;
   string wymagania;
   ///co na questach...
   ///zniszcz 40ramów
   ///wymaksuj wieże
   ///
};

class menu_format
{
public:
    Sprite tlo;
    Texture teksture;
    string nazwa;
    guziki_format guzik[12];
    int ilosc_guzikow=0;
      void nowy_guzik(int xx,int yy,guziki_format przyklad, string komunitant,int nrek)
    {
        guzik[nrek]=przyklad;
        guzik[nrek].x=xx;
        guzik[nrek].y=yy;
        guzik[nrek].komunikat=komunitant;
        guzik[nrek].ustaw_sie();
        ilosc_guzikow++;
    }

};
class bron_format
{
public:
    Sprite obraz;
    int nr;
    int x,y;
    int load;
    int wait;
    int pocisk_max;
    int l_p;
    int ID_naboj;
    int magazyn;
    int czas_reload;
    int odrzut;
    int moc;
    bool reload=false;
};
bron_format miecz;
bron_format pistol;
class pocisk_format
{
    public:
    string nazwa;
    int x,y;
    Sprite pocisk;
    bool moment;
    int speed;
    bool wlocie=true;
    float sila;
};
pocisk_format mm20;
pocisk_format mm120;
pocisk_format mm130;

class artefakty_format
{
public:

   Sprite img;
   int cena;
   Sprite nalozone;
   string nazwa="";
   bool bron=false;
   bool zbroja=false;
   bool szyja=false;
   bool branza=false;
   bool buty=false;
   bool potion=false;
   bool ammo=false;
   int odliczanie;
   bool start=false;
   int poziom=0;
   string typ;
   float bonus;
   void ustaw_artefakt(int rodzaj,string nowa_nazwa,int nowacena,string type,int mocy,int x,int y,int ID,int kolejny)
   {
   cena=nowacena;
   nazwa=nowa_nazwa;
   if(rodzaj==0)
   {    bron=true;
        nalozone.setTexture(nadbroniowa);
        nalozone.setTextureRect(IntRect(kolejny*300,0,300,400));
        nalozone.setOrigin(90,200);
   }
   if(rodzaj==1)
   {    nalozone.setTexture(zbrojowa);
        nalozone.setTextureRect(IntRect(kolejny*80,0,80,120));
        zbroja=true;
   }
   if(rodzaj==2)szyja=true;
   if(rodzaj==3)branza=true;
   if(rodzaj==4)buty=true;
   if(rodzaj==5)potion=true;
   if(rodzaj==6)ammo=true;
   typ=type;
   bonus=mocy;
   img.setTexture(artefaktowa);
   img.setTextureRect(IntRect(x*75,y*75,75,75));
   }

   void umrzyj()
   {
    cena=0;
   nazwa="";
   typ="";
   bonus=0;
   img.setTextureRect(IntRect(0,0,0,0));
   }

};
artefakty_format artefakty[100];

class wieze_format
{
public:
Sprite obraz;
Texture tekstura;
bool wziete=false;
string nazwa;

int reload;
float sila;
int cena;
int zasieg;
int ammo;
int x,y,w,h;
int kontrolx;
int priorytet;
int pozycja;
pocisk_format pocisk;
pocisk_format pocisk_rama;

 void kontrolix()
    {
        kontrolx=x-700;
    }

void przesun(int a, int b)
{
    x+=a;
    y+=b;
    obraz.setPosition(x,y);
}
void wystrzel()
{
    pocisk.wlocie=true;
    Vector2f xy=obraz.getPosition();
    int x=xy.x+30;
    int y=xy.y+30;
    pocisk.x=x;
    pocisk.y=y;
    if(y>600)pocisk.moment=false;
    else pocisk.moment=true;
    pocisk.pocisk.setPosition(x,y);
    pocisk.pocisk.setRotation(90);

}


};
wieze_format wiezowce[20];

class gracz_format
{
    public:

    int ilosc_czesci_ciala;
    Sprite obraz[10];
    bron_format bron;
    Texture tekstura[2];
    int x[10],y[10];
    int w,h;
    int speed;
    bool widac=true;
    bool zwrot_prawo;
    int zmienna_mordu=0;
    int zmienna_miecza=0;
    int licznik_furii=400;
    int licznik_many=400;
    float licznik_blyskawicy=189;
    bool furia=false;
    bool atak_miecza=false;
    bool atak_pistol=true;
    bool blyskawica_start=false;
    int zmienna_miny=0;
    bool wiecej_mocy;
    bool wstep[17];
    bool atak_mina=false;
    bool zyje=true;
    string wieze[6];
    int l_wiez=0;
    int pomocnik[2];
    int hp_max;
    int hp;
    int exp;
    int level;
    float szybkosc_blyskawicy=1;
    float sila;
    float def;
    float pamietnik=def;  ///Furia podnosi do 0.9 wiec trzeba tam jakos wrocic
    int piniondz;
    int ptk_nauki;
    pocisk_format pocisk_wlasny;
    artefakty_format ekwipunek[30];
    artefakty_format bron_ID;
    artefakty_format zbroja;
    artefakty_format szyja;
    artefakty_format branza;
    artefakty_format buty;
    artefakty_format ammo_typ;
    artefakty_format szybkie[6];
    bool pietro_wstep[4];


    void ustaw(int a,int b,int nr)
    {
        x[nr]+=a;
        y[nr]+=b;
        obraz[nr].setPosition(x[nr],y[nr]);
    }

    void uzyj(int numer)
    {

        if(szybkie[numer].potion)
        {
            if(szybkie[numer].typ=="hp")
            {
                hp+=szybkie[numer].bonus;
                if(hp>hp_max)hp=hp_max;
            }
            if(szybkie[numer].typ=="sila")
            {
                sila+=sila/4;
            }
            if(szybkie[numer].typ=="mana")
            {
                licznik_many+=szybkie[numer].bonus;
                if(licznik_many>400) licznik_many=400;
            }
            if(szybkie[numer].typ=="furia")
            {
                licznik_furii+=szybkie[numer].bonus;
                if(licznik_furii>400) licznik_furii=400;
            }
            szybkie[numer].umrzyj();
        }

    }
    void a_sie_wkurwilem()                              ///Furia
    {
                obraz[2].setColor(Color(255,0,0));
                furia=true;
                sila*=4;
                pamietnik=def;
                if(def<0.7)
                {
                    def=0.95;
                }
                else
                {
                    def=1;
                }

    }
    void dobra_spokoj()                             ///koniec furii
    {
        obraz[2].setColor(Color(255,255,255));
        furia=false;
        def=pamietnik;
        sila/=4;

    }
    void szybkie_ubierz(int numer,artefakty_format & art)
    {
        szybkie[numer]=art;
        int xx,yy;
        xx=592+(77*(numer%3));
        yy=1017+(76*(numer/3));
        szybkie[numer].img.setPosition(xx,yy);
        art.umrzyj();
    }

    void krok(int a,int b)
    {
        for(int i=0;i<ilosc_czesci_ciala;i++)
        {
        x[i]+=a;
        y[i]+=b;
        obraz[i].setPosition(x[i],y[i]);
        }
        if(atak_pistol)bron.obraz.setPosition(x[1]+bron.x,y[1]+bron.y);
        else bron.obraz.setPosition(x[1],y[1]);

        if(zwrot_prawo)zbroja.nalozone.setPosition(x[2]+2,y[2]+58);
        else zbroja.nalozone.setPosition(x[2]+76,y[2]+60);

        if(zwrot_prawo)bron_ID.nalozone.setPosition(x[1]+55,y[1]+31);
        else bron_ID.nalozone.setPosition(x[1]-55,y[1]+30);
                         ///trzeba dołożyć jeszcze

    }
     void anim0()
    {
        anim4();
    }

    void anim1()
    {
        obraz[0].setRotation(30);
         obraz[3].setRotation(-30);
    }
    void anim2()
    {
        obraz[0].setRotation(20);
         obraz[3].setRotation(-20);
    }
    void anim3()
    {
        obraz[0].setRotation(10);
         obraz[3].setRotation(-10);
    }
    void anim4()
    {
        obraz[0].setRotation(0);
         obraz[3].setRotation(0);
    }
    void anim7()
    {
        obraz[0].setRotation(-30);
         obraz[3].setRotation(30);
    }
    void anim6()
    {
        obraz[0].setRotation(-20);
         obraz[3].setRotation(20);
    }
    void anim5()
    {
        obraz[0].setRotation(-10);
         obraz[3].setRotation(10);
    }

    void animuj(int nr)
    {
        if(nr==0)anim1();
        if(nr==1||nr==11)anim2();
        if(nr==2||nr==10)anim3();
        if(nr==3||nr==9)anim4();
        if(nr==4||nr==8)anim5();
        if(nr==5||nr==7)anim6();
        if(nr==6)anim7();


    }
    void odwroc(bool prawo)
    {
        if(!prawo)
        {

            bron_ID.nalozone.setScale(-1,1);
            zbroja.nalozone.setScale(-1,1);
            zbroja.nalozone.setPosition(x[2]+76,y[2]+58);
            bron_ID.nalozone.setPosition(x[1]-55,y[1]+30);
            bron.obraz.setRotation(0);
            for(int i=0;i<ilosc_czesci_ciala;i++)obraz[i].setTexture(tekstura[0]);
        zwrot_prawo=false;
            x[4]-=20;
            x[1]-=20;
            x[0]-=30;
            x[3]-=20;
            y[4]-=10;
            obraz[4].setPosition(x[4],y[4]);
            obraz[0].setPosition(x[0],y[0]);
            obraz[3].setPosition(x[3],y[3]);
            obraz[4].setRotation(60);
            obraz[1].setRotation(60);

            if(atak_pistol)
            {bron.obraz.setTexture(broniowa_lewa);
            bron.x=-40;
            bron.obraz.setPosition(x[1]+bron.x,y[1]+bron.y);


            }
                if(atak_miecza)
            {
                bron.obraz.setOrigin(-64+24,-31+173);
                bron.obraz.setScale(-1,1);

            }
        }
        else if(prawo)
        {
            zbroja.nalozone.setScale(1,1);
            bron_ID.nalozone.setScale(1,1);
            zbroja.nalozone.setPosition(x[2]+2,y[2]+58);
            bron_ID.nalozone.setPosition(x[1]+55,y[1]+33);
            bron.obraz.setRotation(0);
            for(int i=0;i<ilosc_czesci_ciala;i++)obraz[i].setTexture(tekstura[1]);
            x[4]+=20;
            x[1]+=20;
            x[0]+=30;
            x[3]+=20;
            y[4]+=10;
            obraz[4].setPosition(x[4],y[4]);
            obraz[0].setPosition(x[0],y[0]);
            obraz[3].setPosition(x[3],y[3]);
            obraz[4].setRotation(-50);
            obraz[1].setRotation(-50);

            zwrot_prawo=true;

             if(atak_pistol)
            {
            bron.x=50;
             bron.obraz.setPosition(x[1]+bron.x,y[1]+bron.y);
             bron.obraz.setTexture(broniowa_prawa);
            }
            if(atak_miecza)
            {
                bron.obraz.setOrigin(-64+24,-31+173);
                bron.obraz.setScale(1,1);

            }
        }

    }
    void zaloz_miecz()
    {
        atak_miecza=true;
        atak_pistol=false;
        bron.obraz=miecz.obraz;
        bron.obraz.setOrigin(-64+24,-31+173);
        bron.obraz.setPosition(x[1],y[1]);

    }
    void zaloz_pistol()
    {
        atak_miecza=false;
        atak_pistol=true;
        bron.obraz=pistol.obraz;
    }
    void usun_bonus(artefakty_format art)
    {
 if(art.typ=="wiedza")
        {
            hp_max-=art.bonus;
            hp-=art.bonus;
        }
        else if(art.typ=="sila")
        {
            sila-=art.bonus;
        }
        else if(art.typ=="szybkosc")
        {
            speed-=art.bonus;
        }
        else if(art.typ=="obrona")
        {
            float help=def*1000;
            help-=art.bonus;
            help=help/1000;
            def=help;
        }
        else if(art.typ=="ammo")
        {
            pocisk_wlasny=mm20;
        }
    }
    void dodaj_bonus (artefakty_format art)
    {
        if(art.typ=="wiedza")
        {
            hp_max+=art.bonus;
            hp+=art.bonus;
        }
        else if(art.typ=="sila")
        {
            sila+=art.bonus;
        }
        else if(art.typ=="szybkosc")
        {
            speed+=art.bonus;
        }
        else if(art.typ=="obrona")
        {
            float help=def*1000;
            help+=art.bonus;
            help=help/1000;
            def=help;
        }
          else if(art.typ=="ammo")
        {
            if(art.bonus==120)pocisk_wlasny=mm120;
            if(art.bonus==130)pocisk_wlasny=mm130;

        }
    }

    ///Funkcje strategiczne

    void rozbierz (string typ)
    {/*
    artefakty_format bron_ID;
    artefakty_format zbroja;
    artefakty_format szyja;
    artefakty_format branza;
    artefakty_format buty;
*/
        int pierwszy_wolny=-1;
        for(int i=0;i<30;i++)if(ekwipunek[i].nazwa=="")
            {
                pierwszy_wolny=i;
                i=31;
                cout<<pierwszy_wolny<<endl;
            }
            if(typ=="zbroja"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=zbroja;

               usun_bonus(zbroja);
               zbroja.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);
            }
            if(typ=="szyja"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=szyja;

               usun_bonus(szyja);
               szyja.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);
            }
             if(typ=="branza"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=branza;

               usun_bonus(branza);
               branza.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);
            }
             if(typ=="buty"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=buty;

                usun_bonus(buty);
                buty.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);

            }
            if(typ=="bron"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=bron_ID;
                usun_bonus(bron_ID);
                bron_ID.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);

            }
              if(typ=="ammo"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=ammo_typ;
                usun_bonus(ammo_typ);
                ammo_typ.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,647+(pierwszy_wolny/10)*78);

            }
    }
    bool podmien_eq(int kursor_x,int kursor_y)
    {
         int tymczasowe_ID=(((kursor_y-4)*10)+kursor_x);

            if(ekwipunek[tymczasowe_ID].nazwa=="")return false;
             dodaj_bonus(ekwipunek[tymczasowe_ID]);
              artefakty_format przenosnia;
             if(ekwipunek[tymczasowe_ID].zbroja)
             {
                    przenosnia=zbroja;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*78);
                    zbroja=ekwipunek[tymczasowe_ID];

             }
             else  if(ekwipunek[tymczasowe_ID].branza)
             {
                    przenosnia=branza;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*78);
                    branza=ekwipunek[tymczasowe_ID];

             }

             else  if(ekwipunek[tymczasowe_ID].bron)
             {
                    przenosnia=bron_ID;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*78);
                    bron_ID=ekwipunek[tymczasowe_ID];


             }
             else  if(ekwipunek[tymczasowe_ID].buty)
             {
                    przenosnia=buty;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*78);
                    buty=ekwipunek[tymczasowe_ID];

            }

              else  if(ekwipunek[tymczasowe_ID].szyja)
            {
                    przenosnia=szyja;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*76);
                    szyja=ekwipunek[tymczasowe_ID];

             }
              else  if(ekwipunek[tymczasowe_ID].ammo)
            {
                    przenosnia=ammo_typ;
                    przenosnia.img.setPosition(87+kursor_x*78,647+(kursor_y-4)*76);
                    ammo_typ=ekwipunek[tymczasowe_ID];
             }

                zbroja.img.setPosition(969,668);
                branza.img.setPosition(1271,668);
                bron_ID.img.setPosition(969,748);
                buty.img.setPosition(969,824);
                szyja.img.setPosition(1271,748);
                ammo_typ.img.setPosition(1271,824);
             ekwipunek[tymczasowe_ID]=przenosnia;
             usun_bonus(przenosnia);
             return true;
    }

void sprawdz_level()
{
    for(int i=0;i<10;i++)
    {
        if(exp-level_limit[level+1]>=0)
    {
    exp-=level_limit[level+1];
    level++;
    ptk_nauki+=5;

    }
else i=10;
    }
}

};
class mobek_format
{
public:
    bool zyje=true;
    bool zdech=false;
    String nazwa;
    int nr_anim=0;
    int max_anim;
    Texture tekstura;
    Sprite img;
    Sprite dol;
    Texture ataku;
    int x,y,w,h;
    int speed;
    int damage;
    int range;
    float hp;
    int exp;
    int piniondz;
    int nr_ataku=0;
    int ilosc_animacji_ataku=10;
    int zmienna_mordu=0;
    bool ruch(gracz_format gracz)
    {
        if(img.getColor()!=Color(255,255,255)) img.setColor(Color(255,255,255));

       /// SZTYWNE USTAWIANIE BEZ MYŚLENIA
        if(gracz.widac)
        {


        if(gracz.x[2]+gracz.w<=x-(w/2)-speed)
        {
           x-=speed;
        img.setPosition(x,y);
        dol.setPosition(x,y+h);
        return false;
        }
       else if(gracz.x[2]>=x+(w/2)+speed)
        {

           x+=speed;
        img.setPosition(x,y);
        dol.setPosition(x,y+h);
         return false;
        }
        else if(gracz.y[2]+gracz.h>=y+speed)
        {

           y+=speed;
        img.setPosition(x,y);
        dol.setPosition(x,y+h);
        return false;
        }
        else if(gracz.y[2]+gracz.h<=y-speed)
        {
           y-=speed;
        img.setPosition(x,y);
        dol.setPosition(x,y+h);
        return false;
        }
        else return true;
        }
        else return false;
    }
    bool uderzony(pocisk_format ammo)
    {
        Vector2f g;
        g=img.getOrigin();
        if(ammo.moment)if(x+g.x>ammo.x-ammo.speed&&x<ammo.x+ammo.speed&&y+(h-g.y)>ammo.y&&y-g.y<ammo.y)return true;
        if(!ammo.moment)if(x-g.x<ammo.x-ammo.speed&&x>ammo.x-ammo.speed&&y+(h-g.y)>ammo.y&&y-g.y<ammo.y)return true;
       return false;
    }


    bool ujmij_zycie(gracz_format & gracz,pocisk_format ammo)
    {
        hp=hp-gracz.sila-gracz.bron.moc*ammo.sila;
        img.setColor(Color(255,0,0));
        if(nazwa=="stopek")img.setColor(Color(0,0,0));
        if(hp<1)
        {
            zyje=false;
            gracz.piniondz+=piniondz;
            gracz.exp+=exp;
            gracz.sprawdz_level();
            smierc.play();
            }
            return !zyje;
    }
    bool ujmij_zycie(gracz_format & gracz)
    {
        hp=hp-gracz.sila-(20*gracz.level)+gracz.bron_ID.bonus;
        img.setColor(Color(255,0,0));
        if(nazwa=="stopek")img.setColor(Color(0,0,0));
        if(hp<1)
            {
            zyje=false;
            gracz.piniondz+=piniondz;
            gracz.exp+=exp;
            gracz.sprawdz_level();
            smierc.play();
            }
            return !zyje;
    }

    bool ujmij_zycie(gracz_format & gracz,pocisk_format ammo,wieze_format wieza)
    {
        hp=hp-wieza.sila*ammo.sila;
        img.setColor(Color(255,0,0));
        if(nazwa=="stopek")img.setColor(Color(0,0,0));
        if(hp<1)
        {
            zyje=false;
            gracz.piniondz+=piniondz;
            gracz.exp+=exp;
           gracz.sprawdz_level();
            smierc.play();

            return !zyje;

            }
    }
    bool atak(gracz_format & gracz)
    {
        if(nr_ataku%ilosc_animacji_ataku==0)
            {
            gracz.hp=gracz.hp-(damage*(1-gracz.def));///MECHANIZM ODEJMOWANIA HP
            if(gracz.def>0.9)gracz.hp--;
            }
        nr_ataku++;
           return !zyje;
    }
    void upadek()
    {
        zmienna_mordu++;//200
        img.move(0,zmienna_mordu-13);
        dol.move(0,zmienna_mordu-13);
        if(zmienna_mordu>50)
        {
            zdech=true;
        }
    }
};
class interfejs_format
{
    public:
    Texture tekstura;
    Texture tlo;
    Sprite obraz[5];
    Font czcionka;
    int ilosc_elementow=5;
    ///pasek hp,many,expa,talary,fala,skille6,
    int x,y,w,h;
    Text naboje_max;
    Text naboje_min;
    Text hajs,level;
    Text exp1,exp2;

    void ustaw_ammo(int ammo,int hajsy)
    {
        naboje_min.setString(intostring(ammo));
        hajs.setString(intostring(hajsy));

    }
    void ustaw_teksty()
    {
        naboje_max.setPosition(920,1025);
        naboje_min.setPosition(860,1025);
        exp1.setPosition(1250,1040);
        hajs.setPosition(1250,1078);
        level.setPosition(1250,1113);

    }
    void ustaw_czcionke ()
    {
        naboje_max.setFont(czcionka);
        naboje_min.setFont(czcionka);
        exp1.setFont(czcionka);
        exp2.setFont(czcionka);
        hajs.setFont(czcionka);
        level.setFont(czcionka);
    }
    void ustaw_zycie(gracz_format gracz)
    {

        float hp=gracz.hp;
        float x=hp/gracz.hp_max;
        obraz[1].setTextureRect(IntRect(0,0,x*400,40));
        obraz[2].setTextureRect(IntRect(0,50,gracz.licznik_many,40));
        obraz[3].setTextureRect(IntRect(0,100,gracz.licznik_furii,40));
        obraz[4].setTextureRect(IntRect(0,0,107,gracz.licznik_blyskawicy));
    }
    void ustaw_staty(gracz_format & gracz)
    {
        exp1.setString(intostring(gracz.exp));
        hajs.setString(intostring(gracz.piniondz));
        level.setString(intostring(gracz.level));
    }



};




class NPC_format
{
    public:
    Sprite obraz;
    Texture tekstura[2];
    string nazwa;
    int x,y;
    int w,h;
    int kontrolx;
    string typ_handlu;
    int ilosc_guzikow;
    guziki_format guzik[12];
    Sprite tlo1,tlo2;
    Texture tlo_tekstura1,tlo_tekstura2;
    Text napisy[15];
    artefakty_format ulepszany;
    int koszty[10];

     void nowy_guzik(int xx,int yy,guziki_format przyklad, string komunitant,int nrek)
    {
        guzik[nrek]=przyklad;
        guzik[nrek].x=xx;
        guzik[nrek].y=yy;
        guzik[nrek].komunikat=komunitant;
        guzik[nrek].ustaw_sie();
    }

    void anim1()
    {
        obraz.setTexture(tekstura[0]);
    }
    void anim2()
    {
       obraz.setTexture(tekstura[1]);
    }
    void ustaw_czcionke (Font & czcionka)
    {
        for(int i =0;i<15;i++)napisy[i].setFont(czcionka);
        for(int i =0;i<15;i++)napisy[i].setCharacterSize(40);
        for(int i =0;i<15;i++)napisy[i].setColor(Color(50,50,50));
    }

     void kontrolix()
    {
        kontrolx=x-700;
    }
     void ustaw_napisy_handel(artefakty_format art)
     {
        napisy[0].setString(art.nazwa);
        napisy[0].setPosition(70,350);
        napisy[1].setString(intostring(art.cena));
        napisy[1].setPosition(70,390);
        napisy[2].setString(art.typ);
        napisy[2].setPosition(70,435);
        if(art.typ=="sila")art.bonus*10;
        napisy[3].setString(intostring(art.bonus));
        napisy[3].setPosition(70,480);

     }
     void ustaw_cene_ulepszenia(artefakty_format art)
     {
        napisy[4].setString("EXP:   "+intostring(art.cena*((art.poziom+1)*0.1)));
        napisy[4].setPosition(440,93);
        napisy[5].setString("HAJS:  "+intostring(art.cena*((art.poziom+1)*0.3)));
        napisy[5].setPosition(440,145);
        if(art.typ=="sila")art.bonus*10;
        napisy[6].setString("PRZED:   "+intostring(art.bonus));
        napisy[6].setPosition(1100,93);
        napisy[7].setString("PO:      "+intostring(art.bonus*0.09+art.bonus));
        napisy[7].setPosition(1100,145);
     }
     void ustaw_staty_gracz(gracz_format gracz)
     {

    napisy[0].setString("MAX ZYCIE      :"+intostring(gracz.hp_max));
    napisy[1].setString("SZYBKOSC       :"+intostring(gracz.speed));
    napisy[2].setString("SILA           :"+(intostring(gracz.sila*10)));
    napisy[3].setString("OBRONA         :"+(intostring(gracz.def*100)+"%"));
    napisy[4].setString("HAJSY          :"+(intostring(gracz.piniondz)));
    napisy[5].setString("LEVEL          :"+(intostring(gracz.level)));
    napisy[6].setString("DOSWIADCZENIE  :"+intostring(gracz.exp));
    napisy[7].setString("PTK NAUKI      :"+intostring(gracz.ptk_nauki));
    napisy[8].setString("NASTEPNY LEVEL :"+intostring(level_limit[gracz.level+1]));
    napisy[9].setString("ILOSC WIEZ     :"+intostring(gracz.l_wiez));
     }
      void ustaw_staty_wiezy(wieze_format wieza)
      {

    napisy[10].setString("NAZWA                :"+wieza.nazwa);
    napisy[11].setString("SILA                 :"+intostring(wieza.sila*10));
    napisy[12].setString("SZYBKOSC STRZALU     :"+intostring(wieza.reload));
    napisy[13].setString("TYP POCISKU          :"+wieza.pocisk_rama.nazwa);

      }
};


class Meble_format
{
    public:
        Sprite obraz;
        Texture tekstura;
        int x,y;
        int w,h;

};

class korytarz_format
{
    public:
        bool anim_wyjscia=false;
        bool anim_wejscia=false;
        int stopien_przezroczystosci=0;
        Sprite tlo;
        Texture tekstura_tla;
        int x=400,y=0;
        NPC_format handlarz[7];
        wieze_format wieze[8];
        int ilosc_blokad;
       int blokadax[10][2];
       int blokaday[10][2];
       string teleport[4];
       string teleport_szczegol[4];
       int teleport_x[4][2];
       int teleport_y[4][2];
       int l_handlarzy;

        bool krok;
        bool zmiana_pietra=false;  ///Jeœli false to zmienia pietro
        void wcisk(string klawisz, int speede)
        {
            if(klawisz=="A")
            {
            x+=speede;
            tlo.setPosition(x,y);

            }

            if(klawisz=="S")
            {
            y-=speede;
            tlo.setPosition(x,y);

            }

            if(klawisz=="D")
            {
            x-=speede;
            tlo.setPosition(x,y);

            }

            if(klawisz=="W")
            {
            y+=speede;
            tlo.setPosition(x,y);

            }
        }


        ///==============================================   TELEPORTY I BLOKADY ==================================================================///

        bool sprawdz(gracz_format xyz, int nr)
        {
            int x1=x;
            int y1=xyz.y[0];
            if(nr==1)x1+=xyz.speed;
            if(nr==2)y1+=xyz.speed;
            if(nr==3)x1-=xyz.speed;
            if(nr==4)y1-=xyz.speed;
            for(int i=0;i<ilosc_blokad;i++)
            {
                if(blokadax[i][0]<=x1&&blokadax[i][1]>=x1&&blokaday[i][0]<=y1&&blokaday[i][1]>=y1)return false;

            }
            krok=true;
            return true;
        }


        void ustaw_blokade (int x,int y,int w,int h,int nr )
        {

        blokadax[nr][0]=x;
        blokadax[nr][1]=w;
        blokaday[nr][0]=y;
        blokaday[nr][1]=h;
        }

        string sprawdz_teleport(string poziom,gracz_format xyz)
        {
            int y1=xyz.y[0];

            for(int i=0;i<4;i++)
            {
               if(teleport_x[i][0]<=x&&teleport_x[i][1]>=x&&teleport_y[i][0]<=y1&&teleport_y[i][1]>=y1)return teleport_szczegol[i];
            }
            return poziom;
        }

        bool sprawdzno_teleport(gracz_format xyz)
        {
            int y1=xyz.y[0];

            for(int i=0;i<4;i++)
            {
               if(teleport_x[i][0]<=x&&teleport_x[i][1]>=x&&teleport_y[i][0]<=y1&&teleport_y[i][1]>=y1)return true;
            }

            return false;
        }

        void ustaw_teleport(int x,int y,int w,int h, string nazwa,string szczegol,int nr)
        {
            teleport[nr]=nazwa;
            teleport_x[nr][0]=x;
            teleport_y[nr][0]=y;
            teleport_x[nr][1]=w;
            teleport_y[nr][1]=h;
            teleport_szczegol[nr]=szczegol;
        }

        ///==================================       HANDLARZE               =========================================================================///
        void przesun_handlarzy(int x)
        {
            for(int i=0;i<l_handlarzy;i++)
            {
                handlarz[i].x+=x;
                 handlarz[i].obraz.setPosition( handlarz[i].x, handlarz[i].y);
            }
        }

         void przesun_wieze(int x)
        {
            for(int i=0;i<8;i++)
            {
                wieze[i].x+=x;
                 wieze[i].obraz.setPosition( wieze[i].x, wieze[i].y);
            }
        }
        int sprawdz_handlarzy(gracz_format xyz)
        {
            int y1=xyz.y[2];

            ///             350 to krokpoczatkowy - szerokosc glowy - 50 to wysokosc
           for(int i=0;i<4;i++)if(700+50>=handlarz[i].x&&700+50<=handlarz[i].x+handlarz[i].w&&handlarz[i].y<=y1&&handlarz[i].y+handlarz[i].h>=y1)return i;
            return -1;
        }
         int sprawdz_wieze(gracz_format xyz)
        {
            int y1=xyz.y[0];
            ///
           for(int i=0;i<3;i++)if(700>=wieze[i].x&&700<=wieze[i].x+wieze[i].w&&wieze[i].y<=y1&&wieze[i].y+wieze[i].h>=y1)return i;
            return -1;
        }
    //700 400
};

class sala_format
{
public:
    Sprite tlo;
    Text falownik_nr;
    short nr_sali;
    short nr_fali;
    bool fala_start;
    bool anim_wyjscia=false;
    bool anim_wejscia=true;
    bool prezentacja=false;
     Sprite pasek_zycia;
     float zycie_bosa;
     Sprite pasek_smierci;
     int stopien_niewidzialnosci=0;
    void ustaw_pasek(float procent)
    {
        pasek_zycia.setTextureRect(IntRect(0,0,400*procent,50));
    }


};

class prezentacja_format
{
public:
    Sprite mobek;
    Sprite tlo;
    Sprite pole_tekstowe;
    Text linijka;
    void ustaw_ogoly(Font & czcionka)
    {
        linijka.setFont(czcionka);
        linijka.setColor(Color(0,0,0,0));
        linijka.setCharacterSize(30);
    }
};

///################################################################################################################################///
/// =======================================================         Funkcje niezależne ============================================///
///################################################################################################################################///

void zapisz_aktualna_gre(gracz_format graczyk,wieze_format tower1,wieze_format tower2,wieze_format tower3,wieze_format tower4,wieze_format tower5)
{
    fstream plik;
    plik.open("save.txt");
    plik<<graczyk.licznik_furii<<endl;
    plik<<graczyk.licznik_many<<endl;
    plik<<graczyk.wstep[0]<<graczyk.wstep[1]<<graczyk.wstep[2]<<graczyk.wstep[3]<<endl;
    plik<<graczyk.hp<<endl;
    plik<<graczyk.hp_max<<endl;
    plik<<graczyk.exp<<endl;
    plik<<graczyk.level<<endl;
    plik<<graczyk.sila*1000<<endl;
    plik<<graczyk.def*1000<<endl;
    plik<<graczyk.piniondz<<endl;
    plik<<graczyk.ptk_nauki<<endl;
    plik<<graczyk.pocisk_wlasny.nazwa<<endl;

    plik<<tower1.nazwa<<endl;
    plik<<tower1.reload<<endl;
    plik<<tower1.pocisk_rama.nazwa<<endl;
    plik<<tower1.sila*10<<endl;
    plik<<tower1.pozycja<<endl;

    plik<<tower2.nazwa<<endl;
    plik<<tower2.reload<<endl;
    plik<<tower2.pocisk_rama.nazwa<<endl;
    plik<<tower2.sila*10<<endl;
    plik<<tower2.pozycja<<endl;

    plik<<tower3.nazwa<<endl;
    plik<<tower3.reload<<endl;
    plik<<tower3.pocisk_rama.nazwa<<endl;
    plik<<tower3.sila*10<<endl;
    plik<<tower3.pozycja<<endl;

    plik<<tower4.nazwa<<endl;
    plik<<tower4.reload<<endl;
    plik<<tower4.pocisk_rama.nazwa<<endl;
    plik<<tower4.sila*10<<endl;
    plik<<tower4.pozycja<<endl;

    plik<<tower5.nazwa<<endl;
    plik<<tower5.reload<<endl;
    plik<<tower5.pocisk_rama.nazwa<<endl;
    plik<<tower5.sila*10<<endl;
    plik<<tower5.pozycja<<endl;

    for(int i=0;i<30;i++)
    {
        plik<<graczyk.ekwipunek[i].nazwa<<endl;
        plik<<graczyk.ekwipunek[i].poziom<<endl;
        int bez_kropek=graczyk.ekwipunek[i].bonus*1000;
        plik<<bez_kropek<<endl;
    }

        plik<<graczyk.bron_ID.nazwa<<endl;
        plik<<graczyk.bron_ID.poziom<<endl;
        plik<<graczyk.bron_ID.bonus*1000<<endl;

        plik<<graczyk.zbroja.nazwa<<endl;
        plik<<graczyk.zbroja.poziom<<endl;
        plik<<graczyk.zbroja.bonus*1000<<endl;

        plik<<graczyk.szyja.nazwa<<endl;
        plik<<graczyk.szyja.poziom<<endl;
        plik<<graczyk.szyja.bonus*1000<<endl;

        plik<<graczyk.branza.nazwa<<endl;
        plik<<graczyk.branza.poziom<<endl;
        plik<<graczyk.branza.bonus*1000<<endl;

        plik<<graczyk.buty.nazwa<<endl;
        plik<<graczyk.buty.poziom<<endl;
        plik<<graczyk.buty.bonus*1000<<endl;

        plik<<graczyk.ammo_typ.nazwa<<endl;
        plik<<graczyk.ammo_typ.poziom<<endl;
        plik<<graczyk.ammo_typ.bonus*1000<<endl;

        for(int i=0;i<6;i++)
        {
            plik<<graczyk.szybkie[i].nazwa<<endl;
        }
        plik.close();

}

void wczytaj_zapisana_gre(gracz_format & graczyk,wieze_format & tower1,wieze_format & tower2,wieze_format & tower3,wieze_format & tower4,wieze_format & tower5);


string komunikator(gracz_format &gracz, string komenda, wieze_format & wieza)
{
    if(komenda=="dodaj_wiezy_sile")
    {
        if(gracz.piniondz-1000>=0){
                wieza.sila+=0.25;
        gracz.piniondz-=1000;

        }

    }
   else if(komenda=="dodaj_wiezy_reload")
    {
        if(gracz.piniondz-1000>=0&wieza.reload-1>3)
        {wieza.reload-=0.25;
        gracz.piniondz-=1000;

        }
    }
   else if(komenda=="dodaj_wiezy_pozycje-")
    {
        wieza.pozycja-=5;

    }
   else if(komenda=="dodaj_wiezy_pozycje+")
    {
        wieza.pozycja+=5;

    }
    else if(komenda=="dodaj_wieze-")
    {
      if(nr_wiezy_przerabianej-1>=0)nr_wiezy_przerabianej--;

    }
    else if(komenda=="dodaj_wieze+")
    {
        if(nr_wiezy_przerabianej+1<=gracz.l_wiez)nr_wiezy_przerabianej++;

    }

return "LOL";
}
string komunikator (gracz_format & gracz,string komenda)
               {

            string funkcja;

            for(int i=0;i<10;i++)if(komenda==intostring(i))
            {tekst_automatu+=komenda;

            }
                   if(komenda.length()>=3)funkcja.insert(0,komenda,0,3);

                                                ///Cena upgrade to 500, trzeba dołożyć punkty nauki
    if(komenda =="dodaj_sile"&&gracz.piniondz-500>=0&&gracz.ptk_nauki-1>=0)
    {
        gracz.sila+=0.25;
    gracz.piniondz-=gracz.level*500;
    gracz.ptk_nauki--;

    }
   else if(komenda=="dodaj_wiedze"&&gracz.piniondz-500>=0&&gracz.ptk_nauki-1>=0){
            gracz.hp_max+=10;
            gracz.hp+=10;
    gracz.piniondz-=gracz.level*500;
    gracz.ptk_nauki--;
    }

   else if(komenda=="dodaj_obrone"&&gracz.piniondz-500>=0&&gracz.def<=0.97&&gracz.ptk_nauki-1>=0){
            gracz.def+=0.01;
    gracz.piniondz-=gracz.level*500;
    gracz.ptk_nauki--;
    }

    else if(komenda=="dodaj_szybkosc"&&gracz.piniondz-500>=0&&gracz.ptk_nauki-1>=0){

    gracz.szybkosc_blyskawicy+=0.5;
    gracz.piniondz-=gracz.level*500;
    gracz.ptk_nauki--;
    }
    else if(funkcja=="KUP")
    {
        string help1;
        string help2;
        help1.insert(0,komenda,3,1);
        help2.insert(0,komenda,4,2);
        int y=stringoint(help1);
        int x=stringoint(help2);
      int ID=x+(y*16);
      if(artefakty[ID].nazwa=="")return "";
        if(gracz.piniondz-artefakty[ID].cena>=0)
        {

            for(int i=0;i<30;i++)if(gracz.ekwipunek[i].nazwa=="")
            {

                gracz.ekwipunek[i]=artefakty[ID];
                gracz.ekwipunek[i].img.setPosition(87+78*(i%10),647+78*(i/10));
                gracz.piniondz=gracz.piniondz-artefakty[ID].cena;
                return "KUPIONY";
            }

            return "BRAK MIEJSCA";
        }
         return "BRAK HAJSU";


    }
     else if(funkcja=="EQG")
     {
        string help1;
        string help2;
        help1.insert(0,komenda,3,1);
        help2.insert(0,komenda,4,2);
        int y=stringoint(help1);
        int x=stringoint(help2);
      int ID=x+(y*10);
       if(gracz.ekwipunek[ID].nazwa!=""){
        gracz.piniondz=gracz.piniondz+gracz.ekwipunek[ID].cena/2;
       gracz.ekwipunek[ID].umrzyj();
         return "SPRZEDANE!";
         }

         return "";
     }
     else if(funkcja=="UPG")
     {
         string help1;
         help1.insert(0,komenda,3,2);                   /// UWAGA TU MOŻE BYĆ BŁĄD!
         artefakty_format art;
         cout<<help1<<funkcja<<endl<<komenda<<endl;                        ///ZE WZGLĘDU NA 1-2 CYFROWE LICZBY
         art=gracz.ekwipunek[stringoint(help1)];
         int p_exp=art.cena*((art.poziom+1)*0.1);
         int p_hajs=art.cena*((art.poziom+1)*0.3);
         int ulepszony=art.bonus*((art.poziom+1)*1.27);

         if(gracz.piniondz-p_hajs>=0&&gracz.exp-p_exp>=0&&art.poziom!=9)
         {
             gracz.piniondz-=p_hajs;
             gracz.exp-=p_exp;
            art.poziom++;
            art.bonus+=art.bonus*0.09;
            gracz.ekwipunek[stringoint(help1)]=art;
            return "ULEPSZONE NA +"+intostring(art.poziom);
         }
         else if(gracz.piniondz-p_hajs<0&&gracz.exp-p_exp<0)return "BRAK HAJSU I EXPA";
         else if(gracz.piniondz-p_hajs<0)return "BRAK HAJSU";
         else if(gracz.exp-p_exp<0)return "BRAK EXPA";




     }
         else if(funkcja=="COF"&&tekst_automatu.length()>0)
         {
             tekst_automatu.erase(tekst_automatu.length()-1,1);
             return "";
         }
         else if(funkcja=="OK:"&&tekst_automatu.length()>0)
         {
             string help1;
        help1.insert(0,komenda,3,2);
        int x=stringoint(help1);
         if(gracz.piniondz-artefakty[x].cena>=0)
        {

            for(int i=0;i<30;i++)if(gracz.ekwipunek[i].nazwa=="")
            {

                gracz.ekwipunek[i]=artefakty[x];
                gracz.ekwipunek[i].img.setPosition(87+78*(i%10),647+78*(i/10));
                gracz.piniondz=gracz.piniondz-artefakty[x].cena;
                return "KUPIONY";
            }

            return "BRAK MIEJSCA";
        }
         return "BRAK HAJSU";



             return "";
         }
    return "MISSED";
}
void animacja_gracz(gracz_format & gracz,string typ)
{
    if(typ=="zdech")
    {
    gracz.zmienna_mordu++;//200
    int x= gracz.zmienna_mordu;
    gracz.krok(0,gracz.zmienna_mordu-13);
    }
    if(typ=="jebs"&&gracz.zwrot_prawo)
    {
        if(gracz.zmienna_miecza<45)
        {

        gracz.obraz[4].setRotation(-50-gracz.zmienna_miecza);
        gracz.obraz[1].setRotation(-50-gracz.zmienna_miecza);
        gracz.bron.obraz.setRotation(-gracz.zmienna_miecza);
        }
       else if(gracz.zmienna_miecza<89)
        {
        gracz.obraz[4].setRotation(-140+gracz.zmienna_miecza);
        gracz.obraz[1].setRotation(-140+gracz.zmienna_miecza);
         gracz.bron.obraz.setRotation(-90+gracz.zmienna_miecza);
        }
        else
        {
        gracz.obraz[4].setRotation(-50);
        gracz.obraz[1].setRotation(-50);

        }

    }
    if(typ=="jebs"&&!gracz.zwrot_prawo)
    {
        if(gracz.zmienna_miecza<45)
        {
        gracz.obraz[4].setRotation(60+gracz.zmienna_miecza);
        gracz.obraz[1].setRotation(60+gracz.zmienna_miecza);
        gracz.bron.obraz.setRotation(gracz.zmienna_miecza);
        }
       else if(gracz.zmienna_miecza<89)
        {
        gracz.obraz[4].setRotation(150-gracz.zmienna_miecza);
        gracz.obraz[1].setRotation(150-gracz.zmienna_miecza);
        gracz.bron.obraz.setRotation(90-gracz.zmienna_miecza);
        }
        else
        {
        gracz.obraz[4].setRotation(60);
        gracz.obraz[1].setRotation(60);

        }
    }
}
void animacja(mobek_format & mobek)
{
    if(mobek.nazwa=="stopek"||mobek.nazwa=="e12"||mobek.nazwa=="STELMACH")
    {
                if(mobek.nr_anim==0)
        {
                    mobek.y-=5;
                    mobek.nr_anim++;

        }
                else if(mobek.nr_anim==1)
        {
            mobek.y-=5;
        }
                else if(mobek.nr_anim==2)
        {
            mobek.y-=5;
        }
                else if(mobek.nr_anim==3)
        {
            mobek.y+=5;
        }
                else if(mobek.nr_anim==4)
        {
            mobek.y+=5;
        }

        else mobek.nr_anim=0;
    }
     if(mobek.nazwa=="ram"||mobek.nazwa=="klawiatura")
    {

                if(mobek.nr_anim==0)
                {
                    mobek.nr_anim++;
                    mobek.img.setRotation(0);
                }
                else if(mobek.nr_anim>=1&&mobek.nr_anim<4)mobek.img.rotate(-5);
                else if(mobek.nr_anim>=4&&mobek.nr_anim<10)mobek.img.rotate(5);
                else if(mobek.nr_anim>=10&&mobek.nr_anim<13)mobek.img.rotate(-5);
                else mobek.nr_anim=0;
    }

    if(mobek.nazwa=="zasilacz")
    {
        if(mobek.nr_anim==0)
                {
                    mobek.nr_anim++;
                    mobek.img.setTexture(zasilaczowa[0]);
                }

        mobek.img.setTexture(zasilaczowa[mobek.nr_anim%4]);

    }
    if(mobek.nazwa=="dysk")
    {
        if(mobek.nr_anim==0)
                {
                    mobek.nr_anim++;
                    mobek.img.setTexture(dyskowa[0]);
                }

        mobek.img.setTexture(dyskowa[mobek.nr_anim%8]);

    }
    if(mobek.nr_anim>0)mobek.nr_anim++;
   // mobek.img.setOrigin(0,0);
}
///################################################################################################################################///
/// =======================================================         Funkcja głowna ================================================///
///################################################################################################################################///

int main()
{
RenderWindow Okno_Calkowite( VideoMode( Szerokosc, Wysokosc, 32 ), "Elektryk THE GAME");//Style::Fullscreen

           /// Ramowka ktora jest wstawiana
Font czcionka;
czcionka.loadFromFile("Grafika//tekst//Track.ttf");
Texture pociskraz;
pociskraz.loadFromFile("Grafika//EQ//pocisk1.png");

///===================================          GUZIKI                    ======================================///
Texture guzik_plusik;
guzik_plusik.loadFromFile("Grafika//menu//dodaj.png");
guziki_format guzik_plus;
guzik_plus.h=60;
guzik_plus.w=60;
guzik_plus.obraz.setTexture(guzik_plusik);

Texture guzik_minusik;
guzik_minusik.loadFromFile("Grafika//menu//minus.png");
guziki_format guzik_minus;
guzik_minus.h=60;
guzik_minus.w=60;
guzik_minus.obraz.setTexture(guzik_minusik);

Texture liczbowa[10];
liczbowa[1].loadFromFile("Grafika//menu//g1.png");
liczbowa[2].loadFromFile("Grafika//menu//g2.png");
liczbowa[3].loadFromFile("Grafika//menu//g3.png");
liczbowa[4].loadFromFile("Grafika//menu//g4.png");
liczbowa[5].loadFromFile("Grafika//menu//g5.png");
liczbowa[6].loadFromFile("Grafika//menu//g6.png");
liczbowa[7].loadFromFile("Grafika//menu//g7.png");
liczbowa[8].loadFromFile("Grafika//menu//g8.png");
liczbowa[9].loadFromFile("Grafika//menu//g9.png");
liczbowa[0].loadFromFile("Grafika//menu//g0.png");

guziki_format guzik_liczba[10];
for(int i=0;i<10;i++)
{
    guzik_liczba[i].obraz.setTexture(liczbowa[i]);
    guzik_liczba[i].w=60;
    guzik_liczba[i].h=60;
}
Texture guzik_okik;
guzik_okik.loadFromFile("Grafika//menu//gok.png");
guziki_format guzik_OK;
guzik_OK.h=60;
guzik_OK.w=60;
guzik_OK.obraz.setTexture(guzik_okik);


Texture guzik_starcik;
guzik_starcik.loadFromFile("Grafika//menu//Start.png");
guziki_format guzik_start;
guzik_start.obraz.setTexture(guzik_starcik);
guzik_start.w=402;
guzik_start.h=128;

Texture guzik_wyjscik;
guzik_wyjscik.loadFromFile("Grafika//menu//exit.png");
guziki_format guzik_wyjscie;
guzik_wyjscie.obraz.setTexture(guzik_wyjscik);
guzik_wyjscie.w=402;
guzik_wyjscie.h=128;
Texture guzik_ustawcik;
guzik_ustawcik.loadFromFile("Grafika//menu//ustawienia.png");
guziki_format guzik_ustawienia;
guzik_ustawienia.obraz.setTexture(guzik_ustawcik);
guzik_ustawienia.w=402;
guzik_ustawienia.h=128;

Texture guzik_wczytnik;
guzik_wczytnik.loadFromFile("Grafika//menu//wczytaj.png");
guziki_format guzik_wczytaj;
guzik_wczytaj.obraz.setTexture(guzik_wczytnik);
guzik_wczytaj.w=402;
guzik_wczytaj.h=128;

Texture guzik_zapisik;
guzik_zapisik.loadFromFile("Grafika//menu//zapisz.png");
guziki_format guzik_zapisz;
guzik_zapisz.h=128;
guzik_zapisz.w=402;
guzik_zapisz.obraz.setTexture(guzik_zapisik);


Texture guzik_cofik;
guzik_cofik.loadFromFile("Grafika//menu//gcof.png");
guziki_format guzik_COF;
guzik_COF.h=60;
guzik_COF.w=60;
guzik_COF.obraz.setTexture(guzik_cofik);

Texture guzik_galerik;
guzik_galerik.loadFromFile("Grafika//menu//galeryjny.png");
guziki_format guzik_galeria;
guzik_galeria.h=249;
guzik_galeria.w=249;
guzik_galeria.obraz.setTexture(guzik_galerik);



///  ==================================           ARTEFAKTY                ==================================== ///
zbrojowa.loadFromFile("Grafika//EQ//zbrojowa.png");
nadbroniowa.loadFromFile("Grafika//EQ//nadbroniowa.png");
szyjowa.loadFromFile("Grafika//EQ//szyjowa.png");
butna.loadFromFile("Grafika//EQ//butna.png");
renczna.loadFromFile("Grafika//EQ//renczna.png");
artefaktowa.loadFromFile("Grafika//EQ//artefaktowa.png");
artefakty[0].ustaw_artefakt(1,"Szara Bluza",1000,"obrona",80,0,0,0,0);
artefakty[1].ustaw_artefakt(1,"Zielona Bluza",2000,"obrona",160,1,0,1,1);
artefakty[2].ustaw_artefakt(1,"Niebieska Bluza",4000,"obrona",240,2,0,2,2);
artefakty[3].ustaw_artefakt(1,"Zolta Bluza",8000,"obrona",320,0,1,3,3);
artefakty[4].ustaw_artefakt(1,"Elektryczna Bluza",16000,"obrona",400,1,1,4,4);
artefakty[5].ustaw_artefakt(1,"Specjalna Bluza",100000,"obrona",450,2,1,5,5);
artefakty[6].ustaw_artefakt(2,"Identyfikator Łachima",1000,"obrona",10,3,0,6,0);
artefakty[7].ustaw_artefakt(2,"Identyfikator Janusza",1000,"wiedza",100,4,0,7,1);
artefakty[8].ustaw_artefakt(4,"Buty Łachima",1000,"szybkosc",5,5,0,8,0);
artefakty[9].ustaw_artefakt(4,"Buty Janusza",1000,"szybkosc",6,6,0,9,1);
artefakty[10].ustaw_artefakt(3,"Zegarek Łachima",1000,"sila",5,7,0,10,0);
artefakty[11].ustaw_artefakt(3,"Zegarek Janusza",5000,"wiedza",200,8,0,11,1);
artefakty[12].ustaw_artefakt(3,"Zegarek Wolaka",10000,"wiedza",600,9,0,12,2);
artefakty[15].ustaw_artefakt(3,"Portfek Seby",1000,"obrona",-200,0,2,15,4);
artefakty[16].ustaw_artefakt(3,"Branza z rezystorow",10000,"obrona",130,9,1,16,3);
artefakty[17].ustaw_artefakt(6,"Kaliber 120",6000,"ammo",120,7,1,17,0);
artefakty[18].ustaw_artefakt(6,"Kaliber 130",6000,"ammo",130,8,1,18,1);
artefakty[13].ustaw_artefakt(0,"Deazert Igl",2000,"sila",5,3,1,13,0);
artefakty[14].ustaw_artefakt(0,"Akacz", 10000,"sila",15,4,1,14,1);
artefakty[19].ustaw_artefakt(0,"Minigun",20000,"sila",20,4,2,19,2);
artefakty[20].ustaw_artefakt(0,"Pistolet z tłumnikiem",4000,"sila",7,5,2,20,4);
artefakty[21].ustaw_artefakt(0,"Zloty Pistol",6000,"sila",10,6,2,21,3);
artefakty[22].ustaw_artefakt(0,"Zloty Akacz",15000,"sila",17,7,2,22,6);
artefakty[23].ustaw_artefakt(0,"Uzi",8000,"sila",12,8,2,23,5);

artefakty[64].ustaw_artefakt(5,"Klapaczos",         1000,"hp",1000,5,1,64,0);
artefakty[65].ustaw_artefakt(5,"Kawa z termosu",    1000,"furia",300,6,1,65,1);
artefakty[66].ustaw_artefakt(5,"Lejs śmietankowe",  600,"hp",400,1,2,66,1);
artefakty[67].ustaw_artefakt(5,"Lejs cebulowe",     700,"hp",500,2,2,67,1);
artefakty[68].ustaw_artefakt(5,"Lejs paprykowe",    800,"hp",600,3,2,68,1);
artefakty[69].ustaw_artefakt(5,"Pepsi",             500,"furia",100,10,0,69,1);
artefakty[70].ustaw_artefakt(5,"Zwolnienie lekarskie",1000,"mana",400,11,0,70,1);
artefakty[71].ustaw_artefakt(5,"Snikers",           750,"furia",200,12,0,71,1);
artefakty[72].ustaw_artefakt(5,"RadioActive",       700,"mana",200,11,1,72,1);
artefakty[73].ustaw_artefakt(5,"Kołacz",            170,"hp",300,12,1,73,1);
artefakty[74].ustaw_artefakt(5,"Chleb",             100,"hp",500,9,2,74,1);
artefakty[75].ustaw_artefakt(5,"Kakao",             600,"mana",100,10,2,75,1);
artefakty[76].ustaw_artefakt(5,"Erka",              900,"mana",350,11,2,76,1);
artefakty[77].ustaw_artefakt(5,"Ciasteczka",        800,"mana",300,2,3,77,1);
artefakty[78].ustaw_artefakt(5,"AntydePRESanty",    1200,"furia",400,3,3,78,1);
artefakty[79].ustaw_artefakt(5,"Ryż z kurczakiem",  1200,"furia",1200,1,3,79,1);
//3  4 1 1
/*
  if(rodzaj==0)bron=true;
   if(rodzaj==1)zbroja=true;
   if(rodzaj==2)szyja=true;
   if(rodzaj==3)branza=true;dw
   if(rodzaj==4)buty=true;
   if(rodzaj==5)potion=true;
   if(rodzaj==6)ammo=true;
    */

///  ==================================           HANDLARZE                ==================================== ///

int dostepne_handle[64];                                /// Edycja dostepności handlu
for(int i=0;i<=23;i++)dostepne_handle[i]=i;                   ///i<ilosc artefaktóq
NPC_format handlarz;
NPC_format handlarz_poty;
handlarz_poty.tlo_tekstura1.loadFromFile("Grafika//EQ//szkolnik.png");
handlarz_poty.tlo1.setTexture(handlarz_poty.tlo_tekstura1);
handlarz_poty.tlo_tekstura2.loadFromFile("Grafika//EQ//wieze_upg.png");
handlarz_poty.tlo2.setTexture(handlarz_poty.tlo_tekstura2);
handlarz_poty.tlo2.setPosition(0,600);
handlarz_poty.tekstura[0].loadFromFile("Grafika//postacie//npc1.png",IntRect(0,0,100,200));
handlarz_poty.tekstura[1].loadFromFile("Grafika//postacie//npc1.png",IntRect(104,0,100,200));
handlarz_poty.x=1100;
handlarz_poty.y=50;
handlarz_poty.w=100;
handlarz_poty.h=200;
handlarz_poty.nazwa="Mistrz Wulfstan";
handlarz_poty.anim1();
handlarz_poty.kontrolix();
handlarz_poty.obraz.setPosition(handlarz_poty.x,handlarz_poty.y);
handlarz_poty.ustaw_czcionke(czcionka);
handlarz_poty.napisy[0].setPosition(340,125);
handlarz_poty.napisy[1].setPosition(340,185);
handlarz_poty.napisy[2].setPosition(340,245);
handlarz_poty.napisy[3].setPosition(340,305);
handlarz_poty.napisy[4].setPosition(300,652);
handlarz_poty.napisy[5].setPosition(300,711);
handlarz_poty.napisy[6].setPosition(300,770);
handlarz_poty.napisy[7].setPosition(300,835);
handlarz_poty.napisy[8].setPosition(1054,130);
handlarz_poty.napisy[9].setPosition(1054,190);
handlarz_poty.guzik[0]=guzik_plus;
handlarz_poty.guzik[0].x=800;
handlarz_poty.guzik[0].y=110;
handlarz_poty.guzik[0].ustaw_sie();
handlarz_poty.guzik[0].komunikat="dodaj_wiedze";

handlarz_poty.guzik[1]=guzik_plus;
handlarz_poty.guzik[1].x=800;
handlarz_poty.guzik[1].y=180;
handlarz_poty.guzik[1].ustaw_sie();
handlarz_poty.guzik[1].komunikat="dodaj_sile";

handlarz_poty.guzik[2]=guzik_plus;
handlarz_poty.guzik[2].x=800;
handlarz_poty.guzik[2].y=250;
handlarz_poty.guzik[2].ustaw_sie();
handlarz_poty.guzik[2].komunikat="dodaj_obrone";

handlarz_poty.guzik[3]=guzik_plus;
handlarz_poty.guzik[3].x=800;
handlarz_poty.guzik[3].y=320;
handlarz_poty.guzik[3].ustaw_sie();
handlarz_poty.guzik[3].komunikat="dodaj_szybkosc";

for(int i=0;i<6;i++)if(i!=3&&i!=2)handlarz_poty.guzik[4+i]=guzik_plus;

handlarz_poty.guzik[8]=guzik_minus;

handlarz_poty.guzik[4].x=754;
handlarz_poty.guzik[4].y=620;
handlarz_poty.guzik[4].komunikat="dodaj_wiezy_sile";

handlarz_poty.guzik[5].x=754;
handlarz_poty.guzik[5].y=690;
handlarz_poty.guzik[5].komunikat="dodaj_wiezy_reload";

handlarz_poty.guzik[8].x=308;
handlarz_poty.guzik[8].y=900;
handlarz_poty.guzik[8].komunikat="dodaj_wieze-";

handlarz_poty.guzik[9].x=400;
handlarz_poty.guzik[9].y=900;
handlarz_poty.guzik[9].komunikat="dodaj_wieze+";

for(int i=0;i<6;i++)if(i!=3&&i!=2)handlarz_poty.guzik[4+i].ustaw_sie();

handlarz_poty.ilosc_guzikow=10;
for(int i=0;i<4;i++)handlarz_poty.koszty[i]=50;

NPC_format handlarz_zlom;
handlarz_zlom.tlo_tekstura1.loadFromFile("Grafika//EQ//handlarz_e.png");
handlarz_zlom.tlo_tekstura2.loadFromFile("Grafika//EQ//eq_gracz.png");
handlarz_zlom.tlo1.setTexture(handlarz_zlom.tlo_tekstura1);
handlarz_zlom.tlo2.setTexture(handlarz_zlom.tlo_tekstura2);
handlarz_zlom.tlo2.setPosition(0,600);
handlarz_zlom.tekstura[0].loadFromFile("Grafika//postacie//npc2.png",IntRect(0,0,100,200));
handlarz_zlom.tekstura[1].loadFromFile("Grafika//postacie//npc2.png",IntRect(104,0,100,200));
handlarz_zlom.x=400;//2200;
handlarz_zlom.y=600;
handlarz_zlom.w=100;
handlarz_zlom.h=200;
handlarz_zlom.nazwa="Handlarz Grzesio";
handlarz_zlom.anim1();
handlarz_zlom.kontrolix();
handlarz_zlom.obraz.setPosition(handlarz_zlom.x,handlarz_zlom.y);
handlarz_zlom.ustaw_czcionke(czcionka);
handlarz_zlom.napisy[0].setPosition(110,120);
handlarz_zlom.napisy[1].setPosition(110,180);
handlarz_zlom.napisy[2].setPosition(110,240);
handlarz_zlom.napisy[3].setPosition(110,300);

handlarz_zlom.ilosc_guzikow=0;
for(int i=0;i<4;i++)handlarz_zlom.koszty[i]=50;

NPC_format handlarz_kowal;
handlarz_kowal.tlo_tekstura1.loadFromFile("Grafika//EQ//kowal.png");
handlarz_kowal.tlo_tekstura2.loadFromFile("Grafika//EQ//eq_gracz.png");
handlarz_kowal.tlo1.setTexture(handlarz_kowal.tlo_tekstura1);
handlarz_kowal.tlo2.setTexture(handlarz_kowal.tlo_tekstura2);
handlarz_kowal.tlo2.setPosition(0,600);
handlarz_kowal.tekstura[0].loadFromFile("Grafika//postacie//npc3.png",IntRect(0,0,100,200));
handlarz_kowal.tekstura[1].loadFromFile("Grafika//postacie//npc3.png",IntRect(104,0,100,200));
handlarz_kowal.x=2000;//2200;
handlarz_kowal.y=50;
handlarz_kowal.w=100;
handlarz_kowal.h=200;
handlarz_kowal.nazwa="Kowal";
handlarz_kowal.anim1();
handlarz_kowal.kontrolix();
handlarz_kowal.obraz.setPosition(handlarz_kowal.x,handlarz_kowal.y);
handlarz_kowal.ustaw_czcionke(czcionka);
handlarz_kowal.napisy[0].setPosition(110,120);
handlarz_kowal.napisy[1].setPosition(110,180);
handlarz_kowal.napisy[2].setPosition(110,240);
handlarz_kowal.napisy[3].setPosition(110,300);

NPC_format handlarz_automat;
handlarz_automat.tlo_tekstura1.loadFromFile("Grafika//EQ//automat.png");
handlarz_automat.tlo_tekstura2.loadFromFile("Grafika//EQ//eq_gracz.png");
handlarz_automat.tlo1.setTexture(handlarz_automat.tlo_tekstura1);
handlarz_automat.tlo2.setTexture(handlarz_automat.tlo_tekstura2);
handlarz_automat.tlo2.setPosition(0,600);
handlarz_automat.tekstura[0].loadFromFile("Grafika//postacie//npc4.png",IntRect(0,0,200,300));
handlarz_automat.tekstura[1].loadFromFile("Grafika//postacie//npc4.png",IntRect(200,0,200,300));
handlarz_automat.x=600;//2200;
handlarz_automat.y=0;
handlarz_automat.w=200;
handlarz_automat.h=300;
handlarz_automat.nazwa="Automat";
handlarz_automat.anim1();
handlarz_automat.kontrolix();
handlarz_automat.obraz.setPosition(handlarz_automat.x,handlarz_automat.y);
handlarz_automat.ustaw_czcionke(czcionka);
handlarz_automat.napisy[0].setPosition(1191,38);
for(int i=1;i<10;i++)handlarz_automat.nowy_guzik(1200+((i-1)%3)*70,120+ ((i-1)/3)*70,guzik_liczba[i],intostring(i),i);
handlarz_automat.nowy_guzik(1270,330,guzik_liczba[0],"0",0);
handlarz_automat.nowy_guzik(1200,330,guzik_OK,"OK:",10);
handlarz_automat.nowy_guzik(1340,330,guzik_COF,"COF",11);
handlarz_automat.ilosc_guzikow=12;

NPC_format gracz_eq;
gracz_eq.tlo_tekstura1.loadFromFile("Grafika//EQ//gracz_staty.png");
gracz_eq.tlo_tekstura2.loadFromFile("Grafika//EQ//eq_gracz.png");
gracz_eq.tlo1.setTexture(gracz_eq.tlo_tekstura1);
gracz_eq.tlo2.setTexture(gracz_eq.tlo_tekstura2);
gracz_eq.tlo2.setPosition(0,600);
gracz_eq.nazwa="GraczEQ";
gracz_eq.ustaw_czcionke(czcionka);
gracz_eq.nowy_guzik(200,250,guzik_minus,"dodaj_wieze-",0);
gracz_eq.nowy_guzik(300,250,guzik_plus,"dodaj_wieze+",1);
gracz_eq.ilosc_guzikow=2;
for(int i=0;i<10;i++)gracz_eq.napisy[i].setPosition(1100,i*50);
for(int i=10;i<15;i++)gracz_eq.napisy[i].setPosition(200,(i-10)*50);
//handlarz_automat.nowy_guzik(1270,330,guzik_liczba[0],"0",0);
//handlarz_automat.ilosc_guzikow=12;
///  ==================================           Bronie                 ==================================== ///
broniowa_prawa.loadFromFile("Grafika//postacie//bronie.png");
broniowa_lewa.loadFromFile("Grafika//postacie//bronie2.png");
obosieczna.loadFromFile("Grafika//postacie//bronie3.png");
pistol.nr=0;
pistol.obraz.setTexture(broniowa_prawa);
pistol.obraz.setTextureRect(IntRect(0,pistol.nr*50,200,50));
pistol.obraz.setOrigin(100,40);
pistol.y=30;
pistol.magazyn=30;
pistol.load=pistol.magazyn;
pistol.czas_reload=20;
pistol.l_p=0;
pistol.odrzut=10;
pistol.ID_naboj=0;
pistol.moc=5;

miecz.obraz.setTexture(obosieczna);
miecz.obraz.setTextureRect(IntRect(0,0,200,200));
miecz.obraz.setOrigin(-64+24,-31+173);

///  ==================================           Pociski               ==================================== ///
pocisk_format ammo[50];
pocisk_format mm20;
mm20.pocisk.setTexture(pociskraz);
mm20.pocisk.setTextureRect(IntRect(0,0,10,10));
mm20.speed=25;
mm20.sila=1.2;
mm130.nazwa="4mm";
Texture pociskdwa;
pociskdwa.loadFromFile("Grafika//EQ//pocisk2.png");
mm120.pocisk.setTexture(pociskdwa);
mm120.pocisk.setTextureRect(IntRect(0,0,60,25));
mm120.speed=25;
mm120.sila=5;
mm120.nazwa="120mm";
Texture pocisk3;
pocisk3.loadFromFile("Grafika//EQ//fireball.png");
mm130.nazwa="fireball";
mm130.pocisk.setTexture(pocisk3);
mm130.pocisk.setTextureRect(IntRect(0,0,50,50));
mm130.speed=15;
mm130.sila=4;
//23,24


///===================================          WIEŻE                       ======================================///

//20 wiez
wiezowce[0].nazwa="vasadir";
wiezowce[0].ammo=0;
wiezowce[0].tekstura.loadFromFile("Grafika//postacie//wieza1.png");
wiezowce[0].obraz.setTexture(wiezowce[0].tekstura);
wiezowce[0].x=3000;
wiezowce[0].y=500;
wiezowce[0].w=100;
wiezowce[0].h=200;
wiezowce[0].kontrolix();
wiezowce[0].przesun(0,0);
wiezowce[0].pocisk.pocisk.setTexture(pociskraz);
wiezowce[0].pocisk_rama=mm120;
wiezowce[0].reload=7;
wiezowce[0].sila=2;
wiezowce[0].cena=9000;

wiezowce[1].nazwa="hitler";
wiezowce[1].ammo=0;
wiezowce[1].tekstura.loadFromFile("Grafika//postacie//wieza2.png");
wiezowce[1].obraz.setTexture(wiezowce[1].tekstura);
wiezowce[1].x=1500;
wiezowce[1].y=700;
wiezowce[1].w=100;
wiezowce[1].h=200;
wiezowce[1].kontrolix();
wiezowce[1].przesun(0,0);
wiezowce[1].pocisk.pocisk.setTexture(pociskraz);
wiezowce[1].pocisk_rama=mm120;
wiezowce[1].reload=13;
wiezowce[1].sila=2;
wiezowce[1].cena=3000;


wiezowce[2].nazwa="rosiu";
wiezowce[2].ammo=0;
wiezowce[2].tekstura.loadFromFile("Grafika//postacie//wieza3.png");
wiezowce[2].obraz.setTexture(wiezowce[2].tekstura);
wiezowce[2].x=2400;
wiezowce[2].y=30;
wiezowce[2].w=100;
wiezowce[2].h=200;
wiezowce[2].kontrolix();
wiezowce[2].przesun(0,0);
wiezowce[2].pocisk_rama=mm120;
wiezowce[2].reload=11;
wiezowce[2].sila=20;
wiezowce[2].cena=6000;

///  ==================================           PIETRA                 ==================================== ///

korytarz_format pietro;
korytarz_format pietra[5];

pietra[1].tekstura_tla.loadFromFile("Grafika//tla//korytarz.png");
pietra[1].tlo.setTexture(pietra[1].tekstura_tla);
///blokady
pietra[1].ilosc_blokad=10;
pietra[1].ustaw_blokade(-2340,0,730,170,0);//rama
pietra[1].ustaw_blokade(-2340,1000,730,1100,1);
pietra[1].ustaw_blokade(730,0,760,1100,2);
pietra[1].ustaw_blokade(-2400,0,-2300,1100,3);
pietra[1].ustaw_blokade(-1840,460,-1220,1100,4);
pietra[1].ustaw_blokade(-350,460,230,1100,5);
pietra[1].l_handlarzy=4;
///teleporty
pietra[1].ustaw_teleport(-220,170,-110,180,"sala","1",0);
pietra[1].ustaw_teleport(-1210,160,-1100,180,"sala","2",1);
pietra[1].ustaw_teleport(-1910,160,-1800,180,"sala","3",2);
pietra[1].ustaw_teleport(-2050,160,-1940,180,"sala","4",3);
///handlarze
pietra[1].handlarz[0]=handlarz_poty;
pietra[1].handlarz[1]=handlarz_zlom;
pietra[1].handlarz[2]=handlarz_kowal;
pietra[1].handlarz[3]=handlarz_automat;
///wieże
pietra[1].wieze[0]=wiezowce[0];
pietra[1].wieze[1]=wiezowce[1];
pietra[1].wieze[2]=wiezowce[2];

pietra[2].tekstura_tla.loadFromFile("Grafika//tla//korytarz2.png");
pietra[2].tlo.setTexture(pietra[2].tekstura_tla);
///blokady
pietra[2].ilosc_blokad=10;
pietra[2].ustaw_blokade(-2340,0,730,170,0);//rama
pietra[2].ustaw_blokade(-2340,1000,730,900,1);//dolna
pietra[2].ustaw_blokade(730,0,760,900,2);
pietra[2].ustaw_blokade(-2400,0,-2300,900,3);
pietra[2].ustaw_blokade(-1840,460,-1220,1500,4);
pietra[2].ustaw_blokade(-350,460,230,1500,5);
pietra[2].l_handlarzy=4;
///teleporty
pietra[2].ustaw_teleport(-220,170,-110,180,"sala","5",0);
pietra[2].ustaw_teleport(-1210,160,-1100,180,"sala","6",1);
pietra[2].ustaw_teleport(-1910,160,-1800,180,"sala","7",2);
pietra[2].ustaw_teleport(-2050,160,-1940,180,"sala","8",3);
///handlarze
pietra[2].handlarz[0]=handlarz_poty;
pietra[2].handlarz[1]=handlarz_zlom;
pietra[2].handlarz[2]=handlarz_kowal;
pietra[2].handlarz[3]=handlarz_automat;
///wieże
pietra[2].wieze[0]=wiezowce[0];
pietra[2].wieze[1]=wiezowce[1];
pietra[2].wieze[2]=wiezowce[2];

///  ==================================           MOBY                 ==================================== ///
mobek_format moby[12];
mobek_format mobki[50];

zasilaczowa[0].loadFromFile("Grafika//postacie//zasil.png");
zasilaczowa[1].loadFromFile("Grafika//postacie//zasilac.png");
zasilaczowa[2].loadFromFile("Grafika//postacie//zasila.png");
zasilaczowa[3].loadFromFile("Grafika//postacie//zasilacz.png");

dyskowa[0].loadFromFile("Grafika//postacie//dysk1.png");
dyskowa[1].loadFromFile("Grafika//postacie//dysk2.png");
dyskowa[2].loadFromFile("Grafika//postacie//dysk3.png");
dyskowa[3].loadFromFile("Grafika//postacie//dysk4.png");
dyskowa[4].loadFromFile("Grafika//postacie//dysk5.png");
dyskowa[5].loadFromFile("Grafika//postacie//dysk6.png");
dyskowa[6].loadFromFile("Grafika//postacie//dysk7.png");
dyskowa[7].loadFromFile("Grafika//postacie//dysk8.png");

moby[7].tekstura.loadFromFile("Grafika//Postacie//Stelmach1.png");
moby[7].img.setTexture(moby[7].tekstura);
moby[7].img.setOrigin(160,270);
moby[7].img.setTextureRect(IntRect(0,0,600,680));
moby[7].speed=4;
moby[7].img.setOrigin(255,567);
moby[7].range=20;
moby[7].w=600;
moby[7].h=650;
moby[7].nazwa="STELMACH";
moby[7].max_anim=6;
moby[7].hp=3500;
moby[7].exp=2000;
moby[7].piniondz=1500;
moby[7].damage=105;
moby[7].ilosc_animacji_ataku=12;

moby[6].img.setTexture(dyskowa[0]);
moby[6].img.setOrigin(150,170);
moby[6].img.setTextureRect(IntRect(0,0,300,300));
moby[6].speed=7;
moby[6].range=20;
moby[6].w=300;
moby[6].h=300;
moby[6].nazwa="dysk";
moby[6].max_anim=14;
moby[6].hp=500;
moby[6].exp=1000;
moby[6].piniondz=500;
moby[6].damage=50;
moby[6].ilosc_animacji_ataku=12;

moby[5].tekstura.loadFromFile("Grafika//postacie//klawiatura.png");
moby[5].img.setTexture(moby[5].tekstura);
moby[5].img.setTextureRect(IntRect(0,0,200,300));
moby[5].dol.setTexture(moby[5].tekstura);
moby[5].dol.setTextureRect(IntRect(0,300,200,70));
moby[5].img.setOrigin(100,300);
moby[5].dol.setOrigin(100,350);
moby[5].speed=9;
moby[5].range=20;
moby[5].w=100;
moby[5].h=300;
moby[5].nazwa="klawiatura";
moby[5].max_anim=14;
moby[5].hp=250;
moby[5].exp=500;
moby[5].piniondz=250;
moby[5].damage=30;
moby[5].ilosc_animacji_ataku=12;

moby[4].img.setTexture(zasilaczowa[0]);
moby[4].img.setTextureRect(IntRect(0,0,253,270));
moby[4].dol.setTexture(moby[4].tekstura);
moby[4].dol.setTextureRect(IntRect(10,260,100,70));
moby[4].img.setOrigin(126,270);
moby[4].dol.setOrigin(50,10);
moby[4].speed=7;
moby[4].range=20;
moby[4].w=253;
moby[4].h=270;
moby[4].nazwa="zasilacz";
moby[4].max_anim=14;
moby[4].hp=350;
moby[4].exp=400;
moby[4].piniondz=300;
moby[4].damage=40;
moby[4].ilosc_animacji_ataku=12;

moby[3].tekstura.loadFromFile("Grafika//postacie//e12.png");
moby[3].img.setTexture(moby[3].tekstura);
moby[3].img.setTextureRect(IntRect(0,0,100,140));
moby[3].dol.setTexture(moby[3].tekstura);
moby[3].dol.setTextureRect(IntRect(0,100,100,100));
moby[3].dol.setOrigin(50,50);
moby[3].img.setOrigin(50,50);
moby[3].speed=8;
moby[3].range=20;
moby[3].w=100;
moby[3].h=160;
moby[3].nazwa="e12";
moby[3].max_anim=6;
moby[3].hp=200;
moby[3].exp=500;
moby[3].piniondz=250;
moby[3].damage=30;

moby[2].tekstura.loadFromFile("Grafika//postacie//stop.png");
moby[2].img.setTexture(moby[2].tekstura);
moby[2].img.setTextureRect(IntRect(0,0,100,90));
moby[2].dol.setTexture(moby[2].tekstura);
moby[2].dol.setTextureRect(IntRect(0,100,100,100));
moby[2].dol.setOrigin(50,50);
moby[2].img.setOrigin(50,50);
moby[2].speed=10;
moby[2].w=100;
moby[2].h=100;
moby[2].nazwa="stopek";
moby[2].max_anim=6;
moby[2].hp=100;
moby[2].exp=600;
moby[2].piniondz=30;
moby[2].damage=10;

moby[1].tekstura.loadFromFile("Grafika//postacie//ram.png");
moby[1].img.setTexture(moby[1].tekstura);
moby[1].img.setTextureRect(IntRect(0,0,100,260));
moby[1].dol.setTexture(moby[1].tekstura);
moby[1].dol.setTextureRect(IntRect(10,260,100,70));
moby[1].img.setOrigin(50,250);
moby[1].dol.setOrigin(50,270);
moby[1].speed=7;
moby[1].range=20;
moby[1].w=100;
moby[1].h=260;
moby[1].nazwa="ram";
moby[1].max_anim=14;
moby[1].hp=120;
moby[1].exp=500;
moby[1].piniondz=250;
moby[1].damage=20;
moby[1].ilosc_animacji_ataku=12;



for(int i=1;i<4;i++)moby[i].speed*=1.5;


///  ==================================           PREZENTACJA              ==================================== ///


Texture pole_tekstowa;
pole_tekstowa.loadFromFile("Grafika//menu//pole_tekstowe.png");
int numer_prezentacji=0;
int maks_prezentacja=1;
prezentacja_format prezentacja;
prezentacja_format prezentacje[20];
prezentacje[0].mobek.setTexture(moby[1].tekstura);
prezentacje[0].mobek.setPosition(200,1600);
prezentacje[0].mobek.setTextureRect(IntRect(0,0,100,260));
prezentacje[0].pole_tekstowe.setTexture(pole_tekstowa);
prezentacje[0].ustaw_ogoly(czcionka);
prezentacje[0].linijka.setPosition(900,200);
string teks_prezentacji;
teks_prezentacji=   "Ramy to podstawowe urzadzenia     \n";
teks_prezentacji+=  "komputerowe. Mozna je czesto      \n";
teks_prezentacji+=  "spotkac na egzaminach. Zazwyczaj  \n";
teks_prezentacji+=  "spisac wlasciwocisci, porownac,   \n";
teks_prezentacji+=  " zamontowac. Wlasciwosci ramow to:\n";
teks_prezentacji+=  "Typ pamięci DDR1,2; Pojemnosc[GB];\n";
teks_prezentacji+=  "Czestotliwosc [MHz]; Napiecie [V] \n";
teks_prezentacji+=  "Po za egzaminem ramy sa czesto    \n";
teks_prezentacji+=  "wymieniane ze wzgledu na cene     \n";
prezentacje[0].linijka.setString(teks_prezentacji);

prezentacje[1].mobek.setTexture(moby[2].tekstura);
prezentacje[1].mobek.setPosition(200,1600);
prezentacje[1].mobek.setTextureRect(IntRect(0,0,100,260));
prezentacje[1].pole_tekstowe.setTexture(pole_tekstowa);
prezentacje[1].ustaw_ogoly(czcionka);
prezentacje[1].linijka.setPosition(900,200);
teks_prezentacji=   "Emergency Stop uzywany do awaryjnego\n";
teks_prezentacji+=  "lub do wlaczania zasilania w sali \n";
teks_prezentacji+=  "nr5. Potem wystarczy zresetowac   \n";
teks_prezentacji+=  "korki, wcisnac jeszcza raz, a     \n";
teks_prezentacji+=  "jezeli to tez nie pomaga, odlaczyc\n";
teks_prezentacji+=  "wszystkie komputery i pojsc do    \n";
teks_prezentacji+=  "kogos kto sie na tym zna \n";
prezentacje[1].linijka.setString(teks_prezentacji);

///  ==================================           PIERUN                 ==================================== ///
Texture pierunowa;
pierunowa.loadFromFile("Grafika//menu//pierun.png");
int ilosc_pierunow=5; //na wypadek powiększenia efektu
Sprite pierun[ilosc_pierunow];
for(int i=0;i<ilosc_pierunow;i++)
{
    pierun[i].setTexture(pierunowa);
    pierun[i].setPosition(((Szerokosc/ilosc_pierunow)+1)*i,1800+i%3*50);
}


///  ==================================           SALE                 ==================================== ///


sala_format sale[5][5];
sala_format sala;
Texture salowa[4];
salowa[0].loadFromFile("Grafika//tla//sala.png");
salowa[1].loadFromFile("Grafika//tla//srodek.png");
salowa[2].loadFromFile("Grafika//tla//koniec.png");
salowa[3].loadFromFile("Grafika//tla//sala1.png");


Texture paskowa;
paskowa.loadFromFile("Grafika//menu//interfejs2.png");
Texture tekstura_sali;
tekstura_sali.loadFromFile("Grafika//tla//sala.png");
sale[1][1].falownik_nr.setPosition(Szerokosc/2,20);
sale[1][1].falownik_nr.setFont(czcionka);
sale[1][1].falownik_nr.setColor(Color(0,0,0));
sale[1][1].tlo.setTexture(tekstura_sali);
sale[1][4].pasek_smierci.setTexture(paskowa);
sale[1][4].pasek_smierci.setTextureRect(IntRect(0,150,400,50));
sale[1][4].pasek_smierci.setPosition((Szerokosc-400)/2,0);
sale[1][4].pasek_zycia.setTexture(paskowa);
sale[1][4].pasek_zycia.setTextureRect(IntRect(0,0,400,50));
sale[1][4].pasek_zycia.setPosition((Szerokosc-400)/2,0);
sale[1][1].nr_sali=0;
sale[1][2].nr_sali=1;
sale[1][3].nr_sali=2;
sale[1][4].nr_sali=3;

sale[2][1].falownik_nr.setPosition(Szerokosc/2,20);
sale[2][1].falownik_nr.setFont(czcionka);
sale[2][1].falownik_nr.setColor(Color(0,0,0));
sale[2][1].tlo.setTexture(tekstura_sali);
sale[2][4].pasek_smierci.setTexture(paskowa);
sale[2][4].pasek_smierci.setTextureRect(IntRect(0,150,400,50));
sale[2][4].pasek_smierci.setPosition((Szerokosc-400)/2,0);
sale[2][4].pasek_zycia.setTexture(paskowa);
sale[2][4].pasek_zycia.setTextureRect(IntRect(0,0,400,50));
sale[2][4].pasek_zycia.setPosition((Szerokosc-400)/2,0);
sale[2][1].nr_sali=4;
sale[2][2].nr_sali=5;
sale[2][3].nr_sali=6;
sale[2][4].nr_sali=7;

sale[3][1].falownik_nr.setPosition(Szerokosc/2,20);
sale[3][1].falownik_nr.setFont(czcionka);
sale[3][1].falownik_nr.setColor(Color(0,0,0));
sale[3][1].tlo.setTexture(tekstura_sali);
sale[3][4].pasek_smierci.setTexture(paskowa);
sale[3][4].pasek_smierci.setTextureRect(IntRect(0,150,400,50));
sale[3][4].pasek_smierci.setPosition((Szerokosc-400)/2,0);
sale[3][4].pasek_zycia.setTexture(paskowa);
sale[3][4].pasek_zycia.setTextureRect(IntRect(0,0,400,50));
sale[3][4].pasek_zycia.setPosition((Szerokosc-400)/2,0);
sale[3][1].nr_sali=8;
sale[3][2].nr_sali=9;
sale[3][3].nr_sali=10;
sale[3][4].nr_sali=11;

sale[4][1].falownik_nr.setPosition(Szerokosc/2,20);
sale[4][1].falownik_nr.setFont(czcionka);
sale[4][1].falownik_nr.setColor(Color(0,0,0));
sale[4][1].tlo.setTexture(tekstura_sali);
sale[4][4].pasek_smierci.setTexture(paskowa);
sale[4][4].pasek_smierci.setTextureRect(IntRect(0,150,400,50));
sale[4][4].pasek_smierci.setPosition((Szerokosc-400)/2,0);
sale[4][4].pasek_zycia.setTexture(paskowa);
sale[4][4].pasek_zycia.setTextureRect(IntRect(0,0,400,50));
sale[4][4].pasek_zycia.setPosition((Szerokosc-400)/2,0);
sale[4][1].nr_sali=12;
sale[4][2].nr_sali=13;
sale[4][3].nr_sali=14;
sale[4][4].nr_sali=15;
Sprite strzalka;
Texture strzalkowska;
strzalkowska.loadFromFile("Grafika//menu//strzalka.png");
strzalka.setTexture(strzalkowska);
strzalka.setPosition(1400,315);
int strzalka_pomocnik=0;



///  ==================================           GRACZ                ==================================== ///

gracz_format gracz1;
gracz_format gracz;
gracz1.bron=pistol;
gracz1.ilosc_czesci_ciala=5;
gracz1.speed=10;
gracz1.hp_max=400;
gracz1.hp=gracz1.hp_max;
gracz1.sila=0;
gracz1.def=0.01;
gracz1.piniondz=3000;
gracz1.level=1;
gracz1.szybkosc_blyskawicy=0.05;
gracz1.exp=0;
gracz1.wstep[0]=true;
gracz1.wstep[1]=false;
gracz1.wstep[2]=false;
gracz1.wstep[3]=1;

gracz1.tekstura[0].loadFromFile("Grafika//postacie//nowy_ludzik2.png");
gracz1.tekstura[1].loadFromFile("Grafika//postacie//nowy_ludzik1.png");

gracz1.obraz[0].setTextureRect(IntRect(0,181,100,50));
gracz1.obraz[0].setTexture(gracz1.tekstura[0]);                          ///noga
gracz1.obraz[0].setOrigin(60,0);
gracz1.x[0]=30;
gracz1.y[0]=162;

gracz1.obraz[1].setTextureRect(IntRect(0,0,47,100));
gracz1.obraz[1].setTexture(gracz1.tekstura[0]);
gracz1.obraz[1].setOrigin(17,19);
gracz1.obraz[1].setRotation(30);
gracz1.x[1]=25;
gracz1.y[1]=95;

gracz1.obraz[2].setTextureRect(IntRect(48,0,76,180));            ///tulow
gracz1.obraz[2].setTexture(gracz1.tekstura[0]);

gracz1.obraz[3].setTextureRect(IntRect(0,181,100,50));
gracz1.obraz[3].setTexture(gracz1.tekstura[0]);                          ///noga
gracz1.obraz[3].setOrigin(60,0);
gracz1.x[3]=30;
gracz1.y[3]=162;

gracz1.obraz[4].setTextureRect(IntRect(0,1,47,100));
gracz1.obraz[4].setTexture(gracz1.tekstura[0]);
gracz1.obraz[4].setOrigin(15,15);
gracz1.obraz[4].setRotation(30);
gracz1.x[4]=25;
gracz1.y[4]=95;
gracz1.pocisk_wlasny=mm20;
gracz1.w=60;
gracz1.h=160;



gracz1.krok(700,500);

///===================================          INTERFEJS                      ======================================///
interfejs_format interfejs;
interfejs.czcionka=czcionka;
interfejs.tlo.loadFromFile("Grafika//menu//interfejs.png");
interfejs.tekstura.loadFromFile("Grafika//menu//interfejs2.png");
Texture bateriowa;
bateriowa.loadFromFile("Grafika//menu//bateria.png");
interfejs.obraz[0].setTexture(interfejs.tlo);
interfejs.obraz[0].setPosition(0,1000);
interfejs.obraz[1].setTextureRect(IntRect(0,0,400,40));
interfejs.obraz[1].setTexture(interfejs.tekstura);
interfejs.obraz[1].setPosition(110,1008);
interfejs.obraz[2].setTextureRect(IntRect(0,50,400,40));
interfejs.obraz[2].setTexture(interfejs.tekstura);
interfejs.obraz[2].setPosition(111,1078);
interfejs.obraz[3].setTextureRect(IntRect(0,100,400,40));
interfejs.obraz[3].setTexture(interfejs.tekstura);
interfejs.obraz[3].setPosition(111,1145);
interfejs.obraz[4].setTexture(bateriowa);
interfejs.obraz[4].setPosition(0,1000);
interfejs.obraz[4].setOrigin(0,189);
interfejs.obraz[4].setScale(1,-1);
interfejs.ustaw_czcionke();
interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
interfejs.ustaw_teksty();
///===================================          MENU                            ==================================== ///

Texture tekstura_tla_menu;
tekstura_tla_menu.loadFromFile("Grafika//menu//tlo.png");
menu_format menu;
menu_format menu_glowne;
menu_glowne.nazwa="glowne";
menu_glowne.tlo.setTexture(tekstura_tla_menu);
menu_glowne.nowy_guzik(600,400,guzik_start,"STR",0);
menu_glowne.nowy_guzik(600,530,guzik_wczytaj,"WCZ",1);
menu_glowne.nowy_guzik(600,660,guzik_zapisz,"ZAP",2);
menu_glowne.nowy_guzik(600,790,guzik_ustawienia,"ZMU",3);
menu_glowne.nowy_guzik(600,920,guzik_wyjscie,"WJD",4);
menu_format menu_ustawienia;
menu_ustawienia.tlo.setTexture(tekstura_tla_menu);
menu_ustawienia.nazwa="galeria";
menu_ustawienia.nowy_guzik(20,100,guzik_galeria,"1",0);
menu_ustawienia.nowy_guzik(320,100,guzik_galeria,"2",1);
menu_ustawienia.nowy_guzik(620,100,guzik_galeria,"3",2);
menu_ustawienia.nowy_guzik(920,100,guzik_galeria,"4",3);
menu_ustawienia.nowy_guzik(20,400,guzik_galeria,"5",4);
menu_ustawienia.nowy_guzik(320,400,guzik_galeria,"6",5);
menu_ustawienia.nowy_guzik(620,400,guzik_galeria,"7",6);
menu_ustawienia.nowy_guzik(920,400,guzik_galeria,"8",7);
menu_ustawienia.nowy_guzik(20,700,guzik_galeria,"9",8);
menu_ustawienia.nowy_guzik(320,700,guzik_galeria,"10",9);
menu_ustawienia.nowy_guzik(620,700,guzik_galeria,"11",10);
menu_ustawienia.nowy_guzik(920,700,guzik_galeria,"12",11);
int wybor_galerii;
///===================================          FILTR                           ==================================== ///

Texture filtrowa;
filtrowa.loadFromFile("Grafika//tla//filtr1.png");
Sprite filtr;
filtr.setTexture(filtrowa);
Texture animowo_przejsciowa;
animowo_przejsciowa.loadFromFile("Grafika//tla//anim.png");
Sprite animacje_przejscia;
animacje_przejscia.setTexture(animowo_przejsciowa);

///===================================          MUZYKA                          ==================================== ///
Music smiercioniany[3];
smiercioniany[0].openFromFile("Odglosy//ded1.ogg");

Music filmowiany[3];

filmowiany[0].openFromFile("Odglosy//track1.ogg");
filmowiany[1].openFromFile("Odglosy//track2.ogg");
Music strzelniany[3];
            Music music;
            music.openFromFile("Odglosy//track1.ogg");
            music.setVolume(0.8);
            strzal_dzwiek.openFromFile("Odglosy//pif.ogg");
            strzal_dzwiek.setVolume(0.7);
            smierc.openFromFile("Odglosy//okurwa.ogg");



///  ==================================           ZMIENNE GLOBALNE              ==================================== ///
bool zmien_pietro=false;
int nr_pietra=1;
int nr_sala=0;
int z=0;
string poziom="menu";
string poprzedni_poziom;
menu=menu_glowne;
bool galeria_start=false;
string poziom_szczegoly="1";
string wysokosc_poziomu="1";
int pseudo_fala=0;
int trudnosc=1;
int beforeX,beforeY;
int kursor_x=0;
int kursor_y=0;
int kursor2_x=0;
int kursor2_y=0;
int x,y;
bool turbowyjscie=true;
Clock clock;
wieze_format tower[6];
///  ==================================           Kursor              ==================================== ///
Texture kursorowa;
kursorowa.loadFromFile("Grafika//EQ//kursor.png");
Sprite kursor;
kursor.setTexture(kursorowa);
///  ==================================           Komunikat              ==================================== ///
string tresc_komunikat;
Text sam_komunikat;
Text sam_komunikat2;
sam_komunikat.setFont(czcionka);
sam_komunikat.setCharacterSize(40);
sam_komunikat.setPosition(50,50);
sam_komunikat.setColor(Color(30,30,30));
sam_komunikat2.setFont(czcionka);
sam_komunikat2.setCharacterSize(40);
sam_komunikat2.setPosition(50,50);
sam_komunikat2.setColor(Color(30,30,30));
sam_komunikat.setPosition(30,30);
sam_komunikat2.setPosition(30,80);
Texture komunikat_tekstura;
Sprite komunikat;
komunikat_tekstura.loadFromFile("Grafika//menu//komunikat.png");
komunikat.setTexture(komunikat_tekstura);
komunikat.setPosition(0,0);
bool wyswietl_komunikat=false;          ///Przy grzesiu
///IntRect(150, 0, 35, 100)
int dziwny_licznik_zapoznawczy=0;
int licznik_klatek=0;
gracz=gracz1;
for(int i=1;i<100;i++)level_limit[i]=500*i*i;/// Generator punktów levelu
gracz.sprawdz_level();

///  ==================================           PETLA GLOWNA              ==================================== ///
while( Okno_Calkowite.isOpen() )
    {
        if(licznik_klatek%14500==100)music.play();
        Time t1 = clock.getElapsedTime();
        if(t1.asMilliseconds()>=1000)
        {
            clock.restart();

            }
        Event dzialanie;
            if(!pietro.zmiana_pietra)
            {

                pietro=pietra[nr_pietra];
                pietro.zmiana_pietra=true;
                pietro.tlo.setPosition(pietro.x,pietro.y);
                 interfejs.naboje_max.setString(intostring(gracz.bron.magazyn));

                 if(nr_pietra==1)
                    {gracz.odwroc(true);
                 gracz.odwroc(false);
                    }
            }


         if(poziom=="korytarz")
         {
bool komunikat_wlacz=false;

/// ######################################################################################################## ////
///  ==================================           KORYTARZ              ==================================== ////
/// ######################################################################################################## ////
    if( t1.asMilliseconds()%20==dziwny_licznik_zapoznawczy%20 )     ///ilosc klatek
    {

        licznik_klatek++;
        if(licznik_klatek%100==0)for(int i=0;i<4;i++)pietro.handlarz[i].anim2();
        if(licznik_klatek%100==50)for(int i=0;i<4;i++)pietro.handlarz[i].anim1();
    dziwny_licznik_zapoznawczy++;


    bool przepustnik;



         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {

            ///Jeżeli puszczony przycisk wraca do pozycji wyjsciowej

        if( dzialanie.type == Event::Closed )Okno_Calkowite.close();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::A)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::S)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::D)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::W)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.uzyj(0);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.uzyj(1);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.uzyj(2);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.uzyj(3);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.uzyj(4);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.uzyj(5);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::E)
        {
            poziom="handel";
            handlarz=gracz_eq;
            poziom_szczegoly=handlarz.nazwa;
            zmien_pietro=true;

        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)
        {
            zmien_pietro=true;
            poziom="menu";
            menu=menu_glowne;
        }


        }


        /// Warunki
        /// na okolice teleportu
         if(pietro.sprawdzno_teleport(gracz))
        {
            string x=pietro.sprawdz_teleport(poziom,gracz);
            komunikat_wlacz=true;
            string tekst_hmmm_komunikatu;
             if(gracz.wstep[stringoint(x)-1])tekst_hmmm_komunikatu="    Sala "+x+" Otwarte";
             else tekst_hmmm_komunikatu="    Sala "+x+" Zamkniete";
            sam_komunikat.setString(tekst_hmmm_komunikatu);
        }
        if(pietro.sprawdz_wieze(gracz)>=0)
        {
                    int numeruj_wieze=pietro.sprawdz_wieze(gracz);
                  komunikat_wlacz=true;
                  string tekst_hmmm_komunikatu;
                  tekst_hmmm_komunikatu="    "+pietro.wieze[numeruj_wieze].nazwa+" : "+intostring(pietro.wieze[numeruj_wieze].cena);
            sam_komunikat.setString(tekst_hmmm_komunikatu);
    }
        if(pietro.sprawdz_handlarzy(gracz)>=0)
        {
              komunikat_wlacz=true;
               sam_komunikat.setString("     "+pietro.handlarz[pietro.sprawdz_handlarzy(gracz)].nazwa);
        }

    /// REAKCJA NA KLAWISZE
            pietro.krok=false;



          if( Keyboard::isKeyPressed( Keyboard::A )&&poziom=="korytarz")
            {
                przepustnik= pietro.sprawdz(gracz,1);
               if(przepustnik)
               {
                pietro.wcisk("A",gracz.speed);
                pietro.przesun_handlarzy(gracz.speed);
                pietro.przesun_wieze(gracz.speed);
                }
                if(gracz.zwrot_prawo)gracz.odwroc(false);
                gracz.animuj((licznik_klatek%48)/4);

            }

            if( Keyboard::isKeyPressed( Keyboard::S ) &&poziom=="korytarz")
            {
                    przepustnik= pietro.sprawdz(gracz,2);
                    if(przepustnik)gracz.krok(0,gracz.speed);
                    gracz.animuj((licznik_klatek%48)/4);
            }



          if( Keyboard::isKeyPressed( Keyboard::D ) &&poziom=="korytarz")
            {

                przepustnik= pietro.sprawdz(gracz,3);
               if(przepustnik)
               {
                    pietro.wcisk("D",gracz.speed);
                    pietro.przesun_handlarzy(-gracz.speed);
                    pietro.przesun_wieze(-gracz.speed);
              }
              if(!gracz.zwrot_prawo)gracz.odwroc(true);
                    gracz.animuj((licznik_klatek%48)/4);
            }



           if( Keyboard::isKeyPressed( Keyboard::W ) &&poziom=="korytarz")
            {
                    przepustnik= pietro.sprawdz(gracz,4);
                    if(przepustnik)gracz.krok(0,-gracz.speed);
                    gracz.animuj((licznik_klatek%48)/4);

            }
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F11)
        {
            gracz.piniondz+=1000;
            gracz.exp+=1000;
            gracz.hp=gracz.hp_max;
        }


           if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Return)
            {
                 int wieza_numeruj=pietro.sprawdz_wieze(gracz);
                 if(pietro.sprawdz_handlarzy(gracz)>=0)
                {
                poziom="handel";
                handlarz=pietro.handlarz[pietro.sprawdz_handlarzy(gracz)];
                poziom_szczegoly=handlarz.nazwa;
                zmien_pietro=true;
                }
            else if(wieza_numeruj>=0)             ///Werbunek wiez
            {

                if(!pietro.wieze[wieza_numeruj].wziete&&gracz.piniondz-pietro.wieze[wieza_numeruj].cena>=0){

                gracz.piniondz-=pietro.wieze[wieza_numeruj].cena;
                 for(int i2=0;i2<6;i2++)if(tower[i2].nazwa=="")
                 {
                     tower[i2]=pietro.wieze[wieza_numeruj];
                     tower[i2].x=200+i2*300;
                     tower[i2].y=100;
                     i2=7;
                     gracz.l_wiez++;
                 }

                            pietro.wieze[wieza_numeruj].wziete=true;
                }

            }
            if(pietro.sprawdzno_teleport(gracz))
                {
                string tymczas=pietro.sprawdz_teleport(poziom,gracz);
                if( gracz.wstep[stringoint(tymczas)-1])
                {
                poziom="sala";
                poziom_szczegoly= tymczas;
                zmien_pietro=true;
                }
                }

            }

            if(pietro.x<=-2280)
            {
                    pietro.zmiana_pietra=false;
                    pietro.x=100;
                    nr_pietra++;
            }

            ///MALOWANIE SPRAJTÓW
        Okno_Calkowite.clear( Color( 0, 0, 0 ) );
        interfejs.ustaw_staty(gracz);
        Okno_Calkowite.draw(pietro.tlo);
        for(int i=7;i>=0;i--)Okno_Calkowite.draw(pietro.wieze[i].obraz);                 ///Wiez jest 8
        for(int i=pietro.l_handlarzy;i>=0;i--)Okno_Calkowite.draw(pietro.handlarz[i].obraz);               /// Handlarzy jest 4
        Okno_Calkowite.draw(gracz.obraz[0]);
        Okno_Calkowite.draw(gracz.buty.nalozone);
        Okno_Calkowite.draw(gracz.obraz[1]);
        Okno_Calkowite.draw(gracz.obraz[2]);
         if(gracz.widac&&gracz.zbroja.nazwa!="")Okno_Calkowite.draw(gracz.zbroja.nalozone);
        Okno_Calkowite.draw(gracz.szyja.nalozone);
        Okno_Calkowite.draw(gracz.obraz[3]);
        Okno_Calkowite.draw(gracz.buty.nalozone);
        if(gracz.bron_ID.nazwa=="")Okno_Calkowite.draw(gracz.bron.obraz);
        if(gracz.bron_ID.nazwa!="")Okno_Calkowite.draw(gracz.bron_ID.nalozone);
        Okno_Calkowite.draw(gracz.obraz[4]);
        Okno_Calkowite.draw(gracz.szyja.nalozone);
        for(int i=0;i<interfejs.ilosc_elementow;i++)Okno_Calkowite.draw(interfejs.obraz[i]);
        Okno_Calkowite.draw(interfejs.naboje_max);
        Okno_Calkowite.draw(interfejs.naboje_min);
        Okno_Calkowite.draw(interfejs.hajs);
        Okno_Calkowite.draw(interfejs.exp1);
        Okno_Calkowite.draw(interfejs.exp2);
        Okno_Calkowite.draw(interfejs.level);
        for(int i=0;i<6;i++) Okno_Calkowite.draw(gracz.szybkie[i].img);
        if(komunikat_wlacz)
        {
            Okno_Calkowite.draw(komunikat);
            Okno_Calkowite.draw(sam_komunikat);
        }

    Okno_Calkowite.draw(filtr);
   Okno_Calkowite.display();
    }
         }

/// ######################################################################################################## ////
///  ==================================           HANDEL                ==================================== ////
/// ######################################################################################################## ////

           else if( poziom=="handel")
           {
            interfejs.ustaw_staty(gracz);
            handlarz.obraz.setPosition(59,36);
            Okno_Calkowite.clear( Color( 255, 0, 255 ) );
            Okno_Calkowite.draw(handlarz.tlo1);
            Okno_Calkowite.draw(handlarz.tlo2);
///  ==================================           WULFSTAN               ==================================== ////
            if(handlarz.nazwa=="Mistrz Wulfstan")
            {
            handlarz.napisy[4].setString("SILA "+intostring(tower[nr_wiezy_przerabianej].sila*10));
            handlarz.napisy[5].setString("CZAS "+intostring(tower[nr_wiezy_przerabianej].reload));
            handlarz.napisy[6].setString("POZYCJA "+intostring(tower[nr_wiezy_przerabianej].x));
            handlarz.napisy[7].setString(tower[nr_wiezy_przerabianej].nazwa);
            tower[nr_wiezy_przerabianej].obraz.setPosition(50,631);
        if(zmien_pietro)
        {

            zmien_pietro=false;



        }
         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {
            x=dzialanie.mouseMove.x;
            y=dzialanie.mouseMove.y;
            bool klikacz_myszkacz=false;        ///Zmienna bo coś z przesyłem do funkcji nie działa
            if( dzialanie.type == sf::Event::MouseButtonPressed && dzialanie.mouseButton.button == sf::Mouse::Left )klikacz_myszkacz=true;
            for(int i=0;i<handlarz.ilosc_guzikow;i++)
            {
                string komunikat;
                komunikat=handlarz.guzik[i].sprawdz_najechanie(beforeX,beforeY,klikacz_myszkacz);
               if(komunikat!="")
                {
                    if(i>3)
                    {
                     i=10;
            komunikator(gracz,komunikat,tower[nr_wiezy_przerabianej]);
            handlarz.napisy[4].setString("SILA "+intostring(tower[nr_wiezy_przerabianej].sila*10));
            handlarz.napisy[5].setString("CZAS PRZELADOWANIA "+intostring(tower[nr_wiezy_przerabianej].reload));
            handlarz.napisy[6].setString("POZYCJA"+intostring(tower[nr_wiezy_przerabianej].x+tower[nr_wiezy_przerabianej].pozycja));
            handlarz.napisy[7].setString(tower[nr_wiezy_przerabianej].nazwa);
             tower[nr_wiezy_przerabianej].obraz.setPosition(78,671);
                    }
                   else
                   {
                    i=10;
                    komunikator(gracz,komunikat);
                   }
                }
            }
                beforeX=x;
                beforeY=y;

        }
        handlarz.napisy[0].setString("HP        :"+intostring(gracz.hp_max));
        handlarz.napisy[1].setString("SILA      :"+intostring(gracz.sila*10));
        handlarz.napisy[2].setString("OBRONA    :"+intostring(gracz.def*1000));
        handlarz.napisy[3].setString("SPEED     :"+intostring(gracz.speed));
        handlarz.napisy[8].setString("PTK NAUKI :"+intostring(gracz.ptk_nauki));
        handlarz.napisy[9].setString("KOSZTA    :"+intostring(gracz.level*500));
        interfejs.hajs.setString(intostring(gracz.piniondz));
        Okno_Calkowite.draw(tower[nr_wiezy_przerabianej].obraz);
                }
///  ==================================           Grzesio               ==================================== ////
///  ==================================           Handluje               ==================================== ////
                 if(handlarz.nazwa=="Handlarz Grzesio")
            {
            string przeslana_komenda;

            if(zmien_pietro)
            {
                for(int i=0;i<64;i++)artefakty[i].img.setPosition((i%16)*78+317,(i/16)*79+14);
                zmien_pietro=false;
                kursor_y=4;
              kursor_x=0;
            }


         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Left)kursor_x--;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Down)
        {
            kursor_y++;
            if(kursor_y==4)kursor_x=0;
        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Right)kursor_x++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Up)kursor_y--;

        if(kursor_x<0)kursor_x++;
        if(kursor_y>6)kursor_y--;
        if(kursor_y<0)kursor_y++;
        if(kursor_y<4&&kursor_x>15)kursor_x--;
        if(kursor_y>=4&&kursor_x>9)kursor_x--;

        if(kursor_y<4)kursor.setPosition(317+((kursor_x%16)*78),14+(kursor_y%4)*79);
        else if(kursor_y>=4)kursor.setPosition(87+((kursor_x%10)*78),649+(kursor_y-4)*76);

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Return)
        {
            if(kursor_y<4)przeslana_komenda=komunikator(gracz,"KUP"+intostring(kursor_y)+intostring(kursor_x));
            else przeslana_komenda=komunikator(gracz,"EQG"+intostring(kursor_y-4)+intostring(kursor_x));
        }
        int aktualne_id;
        if(kursor_y<4)aktualne_id=kursor_x+kursor_y*16;
        else aktualne_id=kursor_x+(kursor_y-4)*10;
        if(kursor_y<4)handlarz.ustaw_napisy_handel(artefakty[aktualne_id]);
        else handlarz.ustaw_napisy_handel(gracz.ekwipunek[aktualne_id]);


        if(przeslana_komenda!="")
        {
            sam_komunikat.setString(przeslana_komenda);
            wyswietl_komunikat=1;
        }

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;

        if(!gracz.ekwipunek[aktualne_id].potion)
        {


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.rozbierz("ammo");

        }
        else if (gracz.ekwipunek[aktualne_id].potion&&gracz.ekwipunek[aktualne_id].nazwa!="")
        {
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.szybkie_ubierz(0,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.szybkie_ubierz(1,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.szybkie_ubierz(2,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.szybkie_ubierz(3,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.szybkie_ubierz(4,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.szybkie_ubierz(5,gracz.ekwipunek[aktualne_id]);
        }

         if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift&&kursor_y>=4)gracz.podmien_eq(kursor_x,kursor_y);


        }

                                            interfejs.hajs.setString(intostring(gracz.piniondz));
                        for(int i=0;i<64;i++)Okno_Calkowite.draw(artefakty[dostepne_handle[i]].img);
                }

///  ==================================           Kowal                   ==================================== ////
///  ==================================           Ulepsza                 ==================================== ////
                 if(handlarz.nazwa=="Kowal")
            {
            string przeslana_komenda;

            if(zmien_pietro)
            {
            kursor_y=4;
              kursor_x=0;
            zmien_pietro=false;
            }


        while( Okno_Calkowite.pollEvent( dzialanie ) )
        {

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Left)kursor_x--;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Down)kursor_y++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Right)kursor_x++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Up)kursor_y--;

        if(kursor_x<0)kursor_x++;
        if(kursor_x>9)kursor_x--;
        if(kursor_y>6)kursor_y--;
        if(kursor_y<4)kursor_y++;
        kursor.setPosition(87+((kursor_x%10)*78),649+(kursor_y-4)*76);
        int aktualne_id;
        if(dzialanie.type == Event::KeyReleased&&kursor_x>=0&&kursor_y>=4)

            {

        aktualne_id=kursor_x+(kursor_y-4)*10;
        handlarz.ulepszany=gracz.ekwipunek[aktualne_id];
        handlarz.ustaw_napisy_handel(handlarz.ulepszany);
        handlarz.ustaw_cene_ulepszenia(handlarz.ulepszany);
        handlarz.ulepszany.img.setPosition(875,100);
        interfejs.hajs.setString(intostring(gracz.piniondz));
        handlarz.ustaw_cene_ulepszenia(handlarz.ulepszany);

        }

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Return)
        {
            if(!gracz.ekwipunek[aktualne_id].ammo)przeslana_komenda=komunikator(gracz,"UPG"+intostring(aktualne_id));
        }
        if(!gracz.ekwipunek[aktualne_id].potion)
        {
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.rozbierz("ammo");

        }
        else if (gracz.ekwipunek[aktualne_id].potion&&gracz.ekwipunek[aktualne_id].nazwa!="")
        {
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.szybkie_ubierz(0,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.szybkie_ubierz(1,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.szybkie_ubierz(2,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.szybkie_ubierz(3,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.szybkie_ubierz(4,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.szybkie_ubierz(5,gracz.ekwipunek[aktualne_id]);
        }


        if(przeslana_komenda!="")
        {
            sam_komunikat.setString(przeslana_komenda);
            wyswietl_komunikat=1;
        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift)gracz.podmien_eq(kursor_x,kursor_y);

        }

                }
///  ==================================           AUTOMAT              ==================================== ////
///  ==================================           ZACINA               ==================================== ////

            if(handlarz.nazwa=="Automat")
            {
        if(zmien_pietro)
        {
            kursor_y=4;
              kursor_x=0;
            zmien_pietro=false;
             for(int i=64;i<100;i++)
                {
                   // artefakty[i].img.setPosition((i%4)*78+350,((i-64)/4)*79+80);
                   artefakty[i].img.setPosition((i%4)*90+350,((i-64)/4)*100+80);
                }
        }
         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {
            x=dzialanie.mouseMove.x;
            y=dzialanie.mouseMove.y;
            bool klikacz_myszkacz=false;        ///Zmienna bo coś z przesyłem do funkcji nie działa
            if( dzialanie.type == sf::Event::MouseButtonPressed && dzialanie.mouseButton.button == sf::Mouse::Left )
            {
                 klikacz_myszkacz=true;
            }

            for(int i=0;i<handlarz.ilosc_guzikow;i++)
            {
                string komunikat;
                komunikat=handlarz.guzik[i].sprawdz_najechanie(beforeX,beforeY,klikacz_myszkacz);
               if(komunikat!="")
                {
                    if(komunikat=="OK:")komunikat+=tekst_automatu;
                komunikator(gracz,komunikat);
                if(tekst_automatu.length()>4)tekst_automatu.erase(4,1);
                handlarz.napisy[0].setString(tekst_automatu);
                }

            }


                beforeX=x;
                beforeY=y;
                if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)
                    {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";
                    }


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Left)kursor_x--;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Down)kursor_y++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Right)kursor_x++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Up)kursor_y--;

        if(kursor_x<0)kursor_x++;
        if(kursor_x>9)kursor_x--;
        if(kursor_y>6)kursor_y--;
        if(kursor_y<4)kursor_y++;
        kursor.setPosition(87+((kursor_x%10)*78),649+(kursor_y-4)*76);
        int aktualne_id;
        aktualne_id=kursor_x+(kursor_y-4)*10;

        if(!gracz.ekwipunek[aktualne_id].potion)
        {


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.rozbierz("ammo");

        }
        else if (gracz.ekwipunek[aktualne_id].potion&&gracz.ekwipunek[aktualne_id].nazwa!="")
        {
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.szybkie_ubierz(0,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.szybkie_ubierz(1,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.szybkie_ubierz(2,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.szybkie_ubierz(3,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.szybkie_ubierz(4,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.szybkie_ubierz(5,gracz.ekwipunek[aktualne_id]);
        }



        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&!wyswietl_komunikat)
        {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";

        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;
          if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift)gracz.podmien_eq(kursor_x,kursor_y);


        }
                for(int i=64;i<100;i++)Okno_Calkowite.draw(artefakty[i].img);

            }
///  ==================================           EKWIPUNEK             ==================================== ////
///  ==================================           GRACZA               ==================================== ////

            if(handlarz.nazwa=="GraczEQ")
            {
            handlarz.ustaw_staty_wiezy(tower[nr_wiezy_przerabianej]);
            handlarz.ustaw_staty_gracz(gracz);
        if(zmien_pietro)
        {
            kursor_y=4;
              kursor_x=0;
            zmien_pietro=false;
        }
         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {

            x=dzialanie.mouseMove.x;
            y=dzialanie.mouseMove.y;
            bool klikacz_myszkacz=false;        ///Zmienna bo coś z przesyłem do funkcji nie działa
            if( dzialanie.type == sf::Event::MouseButtonPressed && dzialanie.mouseButton.button == sf::Mouse::Left )
            {
                 klikacz_myszkacz=true;
            }

            for(int i=0;i<handlarz.ilosc_guzikow;i++)
            {
                string komunikat;
                komunikat=handlarz.guzik[i].sprawdz_najechanie(beforeX,beforeY,klikacz_myszkacz);
               if(komunikat!="")komunikator(gracz,komunikat,tower[nr_wiezy_przerabianej]);
            }


                beforeX=x;
                beforeY=y;


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Left)kursor_x--;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Down)kursor_y++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Right)kursor_x++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Up)kursor_y--;

        if(kursor_x<0)kursor_x++;
        if(kursor_x>9)kursor_x--;
        if(kursor_y>6)kursor_y--;
        if(kursor_y<4)kursor_y++;
        kursor.setPosition(87+((kursor_x%10)*78),649+(kursor_y-4)*76);
        int aktualne_id;
        aktualne_id=kursor_x+(kursor_y-4)*10;

       if(!gracz.ekwipunek[aktualne_id].potion)
        {


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.rozbierz("ammo");

        }
        else if (gracz.ekwipunek[aktualne_id].potion&&gracz.ekwipunek[aktualne_id].nazwa!="")
        {
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.szybkie_ubierz(0,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.szybkie_ubierz(1,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.szybkie_ubierz(2,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.szybkie_ubierz(3,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.szybkie_ubierz(4,gracz.ekwipunek[aktualne_id]);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.szybkie_ubierz(5,gracz.ekwipunek[aktualne_id]);
        }



        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&!wyswietl_komunikat)
        {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";

        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift)gracz.podmien_eq(kursor_x,kursor_y);

        }
            }

            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&!wyswietl_komunikat)
        {

                poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";
        }


                for(int i=0;i<handlarz.ilosc_guzikow;i++) Okno_Calkowite.draw(handlarz.guzik[i].obraz);
                for(int i=0;i<15;i++)Okno_Calkowite.draw(handlarz.napisy[i]);
                for(int i=0;i<4;i++) Okno_Calkowite.draw(interfejs.obraz[i]);
           if(handlarz.nazwa!="Mistrz Wulfstan")
           {
            for(int i=0;i<30;i++)Okno_Calkowite.draw(gracz.ekwipunek[i].img);
            Okno_Calkowite.draw(gracz.zbroja.img);
            Okno_Calkowite.draw(gracz.bron_ID.img);
            Okno_Calkowite.draw(gracz.szyja.img);
            Okno_Calkowite.draw(gracz.buty.img);
            Okno_Calkowite.draw(gracz.branza.img);
            Okno_Calkowite.draw(gracz.ammo_typ.img);
            Okno_Calkowite.draw(kursor);
            }
            Okno_Calkowite.draw(interfejs.naboje_max);
            Okno_Calkowite.draw(interfejs.naboje_min);
        Okno_Calkowite.draw(interfejs.hajs);
        Okno_Calkowite.draw(interfejs.exp1);
        Okno_Calkowite.draw(interfejs.exp2);
        Okno_Calkowite.draw(interfejs.level);
        for(int i=0;i<6;i++) Okno_Calkowite.draw(gracz.szybkie[i].img);

        if(wyswietl_komunikat)
        {
            Okno_Calkowite.draw(komunikat);
            Okno_Calkowite.draw(sam_komunikat);
        }
                Okno_Calkowite.display();




           }

/// ######################################################################################################## ////
///  ==================================           SALE                  ==================================== ////
/// ######################################################################################################## ////
         if(licznik_klatek>100000)licznik_klatek=0;
         else if(poziom=="sala"&&!sala.prezentacja)
         {

             if(zmien_pietro)
             {

                 sala.falownik_nr.setString(intostring(sala.nr_fali)+" STOP");
                 sala.falownik_nr.setColor(Color(0,255,0));
                 nr_sala=stringoint( poziom_szczegoly);
                 sala=sale[nr_pietra][nr_sala];
                 sala.tlo.setTexture(salowa[0]);
                 sala.nr_fali=sala.nr_sali*5;
                 sala.anim_wejscia=true;
                 zmien_pietro=false;
                 gracz.krok(-300,300);
                 licznik_klatek=0;
                 sala.fala_start=true;
                for(int i=0;i<31;i++)ammo[i]=gracz.pocisk_wlasny;
                for(int i=0;i<6;i++)
                {
                    tower[i].x=1200-i*150;
                    tower[i].y=100;
                     tower[i].obraz.setPosition(tower[i].x+tower[i].pozycja,tower[i].y);
                }
                sala.prezentacja=true;
            }

            if( t1.asMilliseconds()%20==dziwny_licznik_zapoznawczy%20  )///ilosc klatek
    {

        licznik_klatek++;
        dziwny_licznik_zapoznawczy++;
        if(gracz.furia)                                             ///Kwestie statystyczne
        {

        if(gracz.licznik_furii>0)gracz.licznik_furii--;
        else gracz.dobra_spokoj();

        }
        if(!gracz.widac)
        {
           if(gracz.licznik_many-1>=0)gracz.licznik_many--;
           else gracz.widac=true;

        }

         while( Okno_Calkowite.pollEvent( dzialanie ) )
        {
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F1)gracz.zaloz_pistol();
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F2)gracz.zaloz_miecz();
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F3)
            {
                if(gracz.furia)gracz.dobra_spokoj();
                else gracz.a_sie_wkurwilem();
            }
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F4)gracz.widac=!gracz.widac;


            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F10)smierc.openFromFile("Odglosy//ok.ogg");
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F11)smierc.openFromFile("Odglosy//pif.ogg");
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F9)sala.nr_fali=19;

            ///Jeżeli puszczony przycisk wraca do pozycji wyjsciowej

        if( dzialanie.type == Event::Closed )Okno_Calkowite.close();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&(!sala.fala_start||gracz.zmienna_mordu>100))
        {
            sala.anim_wyjscia=true;
        }

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::D)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::A)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::S)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::W)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::I)
        {
            if(gracz.licznik_blyskawicy>=185)gracz.blyskawica_start=true;

        }

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F12)Okno_Calkowite.close();

        }


            if(gracz.zyje){
          if( Keyboard::isKeyPressed( Keyboard::A ) &&poziom=="sala")
            {
                if(gracz.x[0]-gracz.speed>=122)gracz.krok(-gracz.speed,0);
                gracz.animuj((licznik_klatek%48)/4);
                 if(gracz.zwrot_prawo)gracz.odwroc(false);


            }

             if( Keyboard::isKeyPressed( Keyboard::S ) &&poziom=="sala")
            {

                 if(gracz.y[0]+gracz.speed<=877)gracz.krok(0,gracz.speed);
                gracz.animuj((licznik_klatek%48)/4);


            }
             if( Keyboard::isKeyPressed( Keyboard::D ) &&poziom=="sala")
            {
               if(gracz.x[0]+gracz.speed<=1570)gracz.krok(gracz.speed,0);
                if(!gracz.zwrot_prawo)gracz.odwroc(true);
                gracz.animuj((licznik_klatek%48)/4);

            }

           /* if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift&&!sala.fala_start&&sala.nr_fali<=20)
            {
                sala.fala_start=true;
                pseudo_fala=0;
                sala.falownik_nr.setString(intostring(sala.nr_fali)+" START");
                 sala.falownik_nr.setColor(Color(255,0,0));
            }*/
            if( Keyboard::isKeyPressed( Keyboard::W ) &&poziom=="sala")
            {
                 if(gracz.y[0]-gracz.speed>=420)gracz.krok(0,-gracz.speed);
                gracz.animuj((licznik_klatek%48)/4);

            }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.uzyj(0);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.uzyj(1);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.uzyj(2);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.uzyj(3);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.uzyj(4);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.uzyj(5);




              if( Keyboard::isKeyPressed( Keyboard::O )&&poziom=="sala"&&gracz.atak_pistol)
            {


           if(gracz.bron.load-1>=0&&(licznik_klatek%gracz.bron.odrzut==1)){
            gracz.widac=true;       ///Pocisk wyputy i w;stawie wściekął morde
            int ne=gracz.bron.magazyn -gracz.bron.load;
            ammo[ne].moment=gracz.zwrot_prawo;
            if(!ammo[ne].moment)ammo[gracz.bron.magazyn -gracz.bron.load].pocisk.setScale(-1,1);
            ammo[ne].x=gracz.x[1]+gracz.bron.x;
            ammo[ne].y=gracz.y[1]+gracz.bron.y-20;///Kalibracja lufy
            gracz.bron.load--;
            strzal_dzwiek.play();
            }
            interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
            }
             else if(gracz.l_wiez>0)
            {
                for(int i=0;i<100;i++)if(i==6){}
            }

           if( Keyboard::isKeyPressed( Keyboard::O )&&poziom=="sala"&&gracz.atak_miecza&&gracz.zmienna_miecza==0)gracz.zmienna_miecza+=5;



            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::P&&!gracz.bron.reload &&poziom=="sala")
            {
          gracz.bron.reload=true;
          gracz.bron.l_p=gracz.bron.czas_reload;
            }
            }


        if(gracz.bron.reload&&(licznik_klatek%2==1||licznik_klatek%2==0)&&gracz.zyje)
        {
            gracz.obraz[1].rotate(6);
            gracz.bron.l_p--;

            if(gracz.bron.l_p==0)
            {
                for(int i=0;i<31;i++)ammo[i]=gracz.pocisk_wlasny;
                gracz.bron.reload=false;
                gracz.bron.load=gracz.bron.magazyn;
                interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
            }
        }

        if(gracz.hp<0)gracz.zyje=false;

         if(sala.fala_start)
        {

              if(licznik_klatek%20==1&&sala.nr_fali<20)                                     ///GENERATOR FALI lol ale zjebany
              {

                    // if(pseudo_fala<5)for(int i=10*pseudo_fala;i<10*(pseudo_fala+1);i+=1+((7-(sala.nr_fali%7))))///10 na pseudo fali;
                   // if(pseudo_fala<1for(int i=0;i<50;i+=1+((7-(sala.nr_fali%7))))///10 na pseudo fali;
                       if(pseudo_fala<1)for(int i=0;i<50;i+=1+((4-(sala.nr_fali%5))))///10 na pseudo fali;
                        {
                            int nr_mobka=(i*23)%3;
                            nr_mobka+=(sala.nr_fali/4);
                            if(nr_mobka==0)nr_mobka=2+(sala.nr_fali/4);

                            if(nr_mobka>0&&(sala.nr_fali/4)+3>nr_mobka)
                            {
                            mobki[i]=moby[nr_mobka];
                            mobki[i].damage+=sala.nr_fali*2;           ///wzmocnienie hp i damage
                            mobki[i].hp+=sala.nr_fali*5;
                            mobki[i].nr_anim=i%mobki[i].max_anim;
                            mobki[i].x=Szerokosc+(i%3)*mobki[i].w/2+i*70;
                            mobki[i].y=550+(i%3)*40;
                            if(nr_mobka>maks_prezentacja)maks_prezentacja++;
                            }
                        }
            pseudo_fala++;

              }
              else if(sala.nr_fali==20&&pseudo_fala<1)
              {
                            mobki[10]=moby[7];
                            mobki[10].damage+=sala.nr_fali*2*(mobki[10].damage/10);           ///BOSS STAGE
                            mobki[10].hp+=sala.nr_fali*5*(mobki[10].hp/20);
                            mobki[10].nr_anim=3;
                            mobki[10].x=2000;
                            mobki[10].y=730;
                            sala.zycie_bosa=mobki[10].hp;
                            sala.falownik_nr.setString(mobki[10].nazwa);
                            sala.falownik_nr.setPosition((Szerokosc-400)/2,0);
                            sala.ustaw_pasek(mobki[10].hp/sala.zycie_bosa);
                        pseudo_fala++;
              }

               if(licznik_klatek%8==1)for(int i=0;i<50;i++)if(mobki[i].nazwa!=""&&mobki[i].zyje)             ///Kwestie mobowe
               {
                   animacja(mobki[i]);
                   if(mobki[i].ruch(gracz))
                   {
                        mobki[i].atak(gracz);
                   }
               }



        }

         if(gracz.zyje)                                         ///Kwestie pociskowe
                {

                    for(int i=0;i<gracz.bron.magazyn-gracz.bron.load;i++)if(ammo[i].wlocie)
                    {
                        ammo[i].x=ammo[i].x+(-ammo[i].speed*(1-(ammo[i].moment*2)));
                        ammo[i].pocisk.setPosition(ammo[i].x,ammo[i].y);
                        if(ammo[i].x>1700||ammo[i].x<0)ammo[i].wlocie=false;

                    }
                    for(int i=0;i<gracz.l_wiez;i++)if(tower[i].pocisk.wlocie&&sala.fala_start)   ///pociski wiezy
                    {
                        tower[i].pocisk.y=tower[i].pocisk.y+(-tower[i].pocisk.speed*(1-(tower[i].pocisk.moment*2)));
                        tower[i].pocisk.pocisk.setPosition(tower[i].pocisk.x,tower[i].pocisk.y);
                        if(tower[i].pocisk.y>1300||tower[i].pocisk.y<0)tower[i].pocisk.wlocie=false;
                       // else tower[i].pocisk.wlocie=true;
                    }

                    for(int i=0;i<50;i++)
                    {
                         for(int i2=0;i2<gracz.bron.magazyn -gracz.bron.load;i2++)if(ammo[i2].wlocie)if(mobki[i].uderzony(ammo[i2])&&mobki[i].zyje)
                         {
                            ammo[i2].wlocie=false;
                             if(mobki[i].ujmij_zycie(gracz,ammo[i2]))for(int i3=0;i3<50;i3++)
                             {
                                 if(mobki[i3].zyje&&mobki[i3].nazwa!="")i3=60;
                                 else if(i3==49)
                                 {
                                     sala.fala_start=false;
                                     pseudo_fala=0;

                                 }
                             }
                         }
                         for(int i2=0;i2<gracz.l_wiez;i2++)if(tower[i2].pocisk.wlocie)if(mobki[i].uderzony(tower[i2].pocisk)&&mobki[i].zyje)   ///pociski wiezy
                    {
                            mobki[i].ujmij_zycie(gracz,tower[i2].pocisk,tower[i2]);
                            tower[i2].pocisk.wlocie=false;
                            if( mobki[i].ujmij_zycie(gracz,tower[i2].pocisk,tower[i2])) for(int i3=0;i3<50;i3++)
                             {
                                 if(mobki[i3].zyje&&mobki[i3].nazwa!="")i3=60;
                                 else if(i3==49)
                                 {


                                     sala.fala_start=false;
                                     pseudo_fala=0;

                                 }
                             }
                    }

                    }


               if(licznik_klatek%10==1&&gracz.l_wiez>0)             ///Kwestie wiezowe
            {


            for(int i=0;i<gracz.l_wiez;i++)
            {
                tower[i].ammo++;
                if(tower[i].ammo%tower[i].reload==0)
                {
                    tower[i].pocisk=tower[i].pocisk_rama;
                 tower[i].wystrzel();
                }
            }


            }
             if(gracz.zmienna_miecza>0)                       ///KWESTIE MIECZOWE
            {
                animacja_gracz(gracz,"jebs");
                gracz.zmienna_miecza++;
                if(gracz.zmienna_miecza>=90&&sala.fala_start)
                    {
              for(int i=0;i<50;i++) if(mobki[i].zyje)if(mobki[i].ruch(gracz))if( mobki[i].ujmij_zycie(gracz))for(int i3=0;i3<50;i3++)
                             {
                                 if(mobki[i3].zyje&&mobki[i3].nazwa!="")i3=60;
                                 else if(i3==49)
                                 {


                                     sala.fala_start=false;
                                     pseudo_fala=0;

                                 }
                             }
                    gracz.zmienna_miecza=0;
                }
            }
                                                                            ///KWESTIE BŁYSKAWICY
                    if(gracz.licznik_blyskawicy<189&&!gracz.blyskawica_start)
                    {
                        gracz.licznik_blyskawicy+=gracz.szybkosc_blyskawicy;


                    }
                if(gracz.blyskawica_start)
                {
                    for(int i=0;i<ilosc_pierunow;i++)pierun[i].setPosition(((Szerokosc/ilosc_pierunow)+1)*i,(50-gracz.licznik_blyskawicy)*7+(i%3)*50);
                    if(gracz.licznik_blyskawicy--<1)
                    {
                         for(int i=0;i<50;i++)if(mobki[i].x<Szerokosc)mobki[i].hp=mobki[i].hp-100-(gracz.sila*10);
                         for(int i=0;i<ilosc_pierunow;i++)pierun[i].setPosition(((Szerokosc/ilosc_pierunow)+1)*i,1800+i%3*50);

                        gracz.blyskawica_start=false;
                    }
                }


                for(int i=0;i<50;i++)if(!mobki[i].zyje)mobki[i].upadek();

                 if(!sala.fala_start&&sala.nr_fali!=20)
                 {
                     if(gracz.x[0]>=1400)                                       ///Funkcja ZMIEN SALE
                     {
                         gracz.krok(-1000,100);
                         sala.nr_fali++;
                         if(sala.nr_fali%5==1||sala.nr_fali%5==2||sala.nr_fali%5==3)sala.tlo.setTexture(salowa[1]);
                         else sala.tlo.setTexture(salowa[2]);
                         sala.fala_start=true;
                         if(sala.nr_fali%5==0&&sala.nr_fali!=20)
                         {
                             sala.anim_wyjscia=true;
                             if(sala.nr_sali<4)
                             {
                                 gracz.wstep[sala.nr_sali+1]=true;
                                sale[nr_pietra][sala.nr_sali].prezentacja=true;
                             }
                             else

                             {
                                 gracz.wstep[sala.nr_sali+1]=true;
                                 gracz.pietro_wstep[nr_pietra]=true;
                             }


                         }
                         if(sala.nr_fali==20)
                         {
                             sala.tlo.setTexture(salowa[3]);
                         }
                     }
                 }

                }

                if(!gracz.zyje)
                {
                    animacja_gracz(gracz,"zdech");

                }
                cout<<"Czas: "<<t1.asMilliseconds();
                cout<<"   Klatki:"<<licznik_klatek<<endl;

        interfejs.ustaw_zycie(gracz);
        interfejs.ustaw_staty(gracz);
        sala.falownik_nr.setString(intostring(sala.nr_fali));
        Okno_Calkowite.clear( Color( 0, 0, 0 ) );
        Okno_Calkowite.draw(sala.tlo);
        for(int i=0;i<gracz.l_wiez;i++)
        {
            Okno_Calkowite.draw(tower[i].obraz);
            if(tower[i].pocisk.wlocie)Okno_Calkowite.draw(tower[i].pocisk.pocisk);
        }
        if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[0]);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[1]);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[2]);
         if(gracz.widac&&gracz.zbroja.nazwa!="")Okno_Calkowite.draw(gracz.zbroja.nalozone);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[3]);
        if(gracz.bron_ID.nazwa==""||gracz.atak_miecza)Okno_Calkowite.draw(gracz.bron.obraz);
        if(gracz.bron_ID.nazwa!="")if(!gracz.atak_miecza)Okno_Calkowite.draw(gracz.bron_ID.nalozone);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[4]);
        for(int i=0;i<gracz.bron.magazyn -gracz.bron.load;i++)
        {
            if(ammo[i].wlocie)Okno_Calkowite.draw(ammo[i].pocisk);
        }

         if(sala.fala_start)
        {
            for(int i=0;i<50;i++)if(!mobki[i].zdech)Okno_Calkowite.draw(mobki[i].img);
        }

        if(gracz.blyskawica_start)
        {
            for(int i=0;i<ilosc_pierunow;i++)Okno_Calkowite.draw(pierun[i]);
        }

         for(int i=0;i<interfejs.ilosc_elementow;i++)Okno_Calkowite.draw(interfejs.obraz[i]);
        Okno_Calkowite.draw(interfejs.naboje_max);
        Okno_Calkowite.draw(interfejs.naboje_min);
        Okno_Calkowite.draw(interfejs.hajs);
        Okno_Calkowite.draw(interfejs.exp1);
        Okno_Calkowite.draw(interfejs.exp2);
        Okno_Calkowite.draw(interfejs.level);
        if(sala.nr_fali==20&&sala.fala_start)
                {
                   sala.ustaw_pasek(mobki[10].hp/sala.zycie_bosa);
                    Okno_Calkowite.draw(sala.pasek_smierci);
                    Okno_Calkowite.draw(sala.pasek_zycia);
                    sala.falownik_nr.setString(mobki[10].nazwa);
                    sala.falownik_nr.setColor(Color(0,0,0));
                }
        Okno_Calkowite.draw(sala.falownik_nr);
        for(int i=0;i<6;i++) Okno_Calkowite.draw(gracz.szybkie[i].img);

         if(gracz.zmienna_mordu>100)
                    {
                        gracz.zyje=true;
                        gracz.krok(700-gracz.x[2],200-gracz.y[2]);
                        komunikat.setScale(2,1);
                        Okno_Calkowite.draw(komunikat);
                        sam_komunikat.setString("                 Jak mozna przegrac na "+intostring(sala.nr_fali)+" fali..");
                        Okno_Calkowite.draw(sam_komunikat);
                        komunikat.setScale(1,1);
                    }
         if(sala.nr_fali==20&&!sala.fala_start)
            {
                        komunikat.setScale(2,1);
                        Okno_Calkowite.draw(komunikat);
                        sam_komunikat.setString("                 Zniszczyles stelmacha!");
                        sam_komunikat2.setString("               Zmierzaj dalej, szanowny");
                        Okno_Calkowite.draw(sam_komunikat);
                        Okno_Calkowite.draw(sam_komunikat2);
            }
            if(!sala.fala_start&&sala.nr_fali!=20)
            {
                strzalka.setColor(Color(255,255,255,strzalka_pomocnik++));
                Okno_Calkowite.draw(strzalka);
            }
            if(sala.anim_wyjscia&&sala.stopien_niewidzialnosci<=0)
            {
            poziom="korytarz";
            zmien_pietro=true;
            sala.anim_wyjscia=false;
            sala.anim_wejscia=false;
            gracz.krok(700-gracz.x[2],200-gracz.y[2]);
            poziom_szczegoly="1";
            if(gracz.zmienna_mordu>99)
            {
                gracz.piniondz=0;
                gracz.hp=gracz.hp_max;
                gracz.exp=0;
                gracz.zyje=1;
                gracz.zmienna_mordu=0;
            }
            if(sala.nr_fali==21)
            {
                komunikat.setScale(1,1);
            }

            }
            if(sala.anim_wejscia||sala.anim_wyjscia)
            {
                animacje_przejscia.setColor(Color(255,255,255,255-sala.stopien_niewidzialnosci));
                if(sala.anim_wejscia)sala.stopien_niewidzialnosci++;
                else sala.stopien_niewidzialnosci--;
                if(sala.stopien_niewidzialnosci==255)sala.anim_wejscia=0;
                Okno_Calkowite.draw(animacje_przejscia);
            }
        Okno_Calkowite.draw(filtr);
        Okno_Calkowite.display();

         }

         }
         ///ANIMACJA PREZENTACJA
        else  if((poziom=="sala"&&sala.prezentacja)||galeria_start)
        {
                prezentacja=prezentacje[sala.nr_sali];
                if(galeria_start)prezentacja=prezentacje[wybor_galerii];
                prezentacja.pole_tekstowe.setPosition(900,1200);
                prezentacja.mobek.setOrigin(prezentacja.mobek.getTextureRect().width/2,prezentacja.mobek.getTextureRect().height);
                int czas=t1.asMilliseconds();
                prezentacja.mobek.setScale(3,3);
                int wspolczynnik_widzialnosci=0;
                bool prawo=true;
            while( !Okno_Calkowite.pollEvent( dzialanie ) )
            {

                    t1=clock.getElapsedTime();
                    czas=t1.asMilliseconds();
                    if(czas%20==5)
                    {
                        Okno_Calkowite.clear(Color(239,229,187));
                        Vector2f wektor=prezentacja.pole_tekstowe.getPosition();
                        int x=wektor.x;
                        if(prezentacja.pole_tekstowe.getPosition().y>200)prezentacja.pole_tekstowe.move(0,-20);
                        else if(wspolczynnik_widzialnosci<251)wspolczynnik_widzialnosci+=3;
                        if(prezentacja.mobek.getPosition().y>900)prezentacja.mobek.move(0,-15);
                        if(prezentacja.mobek.getPosition().x<400)prezentacja.mobek.move(15,0);
                        else
                        {
                           if(prawo)
                           {
                               prezentacja.mobek.rotate(0.1);
                               if(prezentacja.mobek.getRotation()>30&&prezentacja.mobek.getRotation()<300)prawo=!prawo;
                           }
                           else
                           {
                               prezentacja.mobek.rotate(-0.1);
                               if(prezentacja.mobek.getRotation()<330&&prezentacja.mobek.getRotation()>50)prawo=!prawo;
                           }
                        }
                        prezentacja.linijka.setColor(Color(0,0,0,wspolczynnik_widzialnosci));

                    Okno_Calkowite.draw(prezentacja.pole_tekstowe);
                    Okno_Calkowite.draw(prezentacja.mobek);
                    Okno_Calkowite.draw(prezentacja.linijka);
                    Okno_Calkowite.display();
                    }
                }
                if(!galeria_start)numer_prezentacji++;
                sala.prezentacja=false;
                galeria_start=false;

        }
/// ######################################################################################################## ////
///  ==================================           MENU                  ==================================== ////
/// ######################################################################################################## ////
else if(poziom=="menu")
{


if(menu.nazwa=="glowne")
    {
    Okno_Calkowite.clear( Color( 255, 255, 255 ) );
     while( Okno_Calkowite.pollEvent( dzialanie ) )
        {
             if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)poziom="korytarz";

             x=dzialanie.mouseMove.x;
            y=dzialanie.mouseMove.y;
            bool klikacz_myszkacz=false;        ///Zmienna bo coś z przesyłem do funkcji nie działa
            if( dzialanie.type == sf::Event::MouseButtonPressed && dzialanie.mouseButton.button == sf::Mouse::Left )
            {
                 klikacz_myszkacz=true;
            }

            for(int i=0;i<menu.ilosc_guzikow;i++)
            {
                string komunikat;
                komunikat=menu.guzik[i].sprawdz_najechanie(beforeX,beforeY,klikacz_myszkacz);
               if(komunikat=="WJD")
               {
               Okno_Calkowite.close();
               turbowyjscie=1;
               }
               if(komunikat=="STR")poziom="korytarz";
               if(komunikat=="ZAP")zapisz_aktualna_gre(gracz,tower[0],tower[1],tower[2],tower[3],tower[4]);
               if(komunikat=="WCZ")wczytaj_zapisana_gre(gracz,tower[0],tower[1],tower[2],tower[3],tower[4]);
               if(komunikat=="ZMU")menu=menu_ustawienia;

            }


                beforeX=x;
                beforeY=y;
        }

    Okno_Calkowite.draw(menu.tlo);
    for(int i=0;i<menu.ilosc_guzikow;i++)Okno_Calkowite.draw(menu.guzik[i].obraz);
    Okno_Calkowite.display();
}
else if(menu.nazwa=="galeria")
    {
         for(int i=0;i<=8;i++)
                {
                    if(i<maks_prezentacja)moby[i].img.setColor(Color(255,255,255));
                    else moby[i].img.setColor(Color(0,0,0));
                }

                moby[1].img.setScale(0.75,0.75);
                moby[1].img.setPosition(menu.guzik[1].x-200,menu.guzik[1].y+200);
                moby[2].img.setPosition(menu.guzik[2].x-200,menu.guzik[2].y+100);
                moby[3].img.setPosition(menu.guzik[3].x-200,menu.guzik[3].y+100);
                moby[4].img.setPosition(menu.guzik[4].x-200,menu.guzik[4].y+200);
                moby[4].img.setScale(0.75,0.75);
                moby[5].img.setPosition(menu.guzik[5].x-200,menu.guzik[5].y+230);
                moby[5].img.setScale(0.75,0.75);
                moby[6].img.setPosition(menu.guzik[6].x-210,menu.guzik[6].y+10);
                moby[6].img.setScale(0.2,0.2);

    Okno_Calkowite.clear( Color( 255, 255, 255 ) );
     while( Okno_Calkowite.pollEvent( dzialanie ) )
        {
             if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)
                {
                menu=menu_glowne;
                for(int i=0;i<8;i++)moby[i].img.setColor(Color(255,255,255));
                for(int i=0;i<8;i++)moby[i].img.setScale(1,1);
                }
             x=dzialanie.mouseMove.x;
            y=dzialanie.mouseMove.y;
            bool klikacz_myszkacz=false;        ///Zmienna bo coś z przesyłem do funkcji nie działa
            if( dzialanie.type == sf::Event::MouseButtonPressed && dzialanie.mouseButton.button == sf::Mouse::Left )
            {
                 klikacz_myszkacz=true;
            }
            for(int i=0;i<menu.ilosc_guzikow;i++)
            {
                string komunikat;
                komunikat=menu.guzik[i].sprawdz_najechanie(beforeX,beforeY,klikacz_myszkacz);
                if(komunikat!="")
                {
                    if(stringoint(komunikat)-1<maks_prezentacja)
                    {
                    galeria_start=true;
                    wybor_galerii=stringoint(komunikat)-1;
                    }
                }
            }


                beforeX=x;
                beforeY=y;
        }

    Okno_Calkowite.draw(menu.tlo);
    for(int i=0;i<menu.ilosc_guzikow;i++)Okno_Calkowite.draw(menu.guzik[i].obraz);
    for(int i=0;i<7;i++)Okno_Calkowite.draw(moby[i].img);
    Okno_Calkowite.display();
}
    }
//Okno_Calkowite.draw(filtr);

    }


    return 0;
}
/// ######################################################################################################## ////
///  ==================================           SAVE                 ==================================== ////
/// ######################################################################################################## ////
void wczytaj_zapisana_gre(gracz_format & graczyk,wieze_format & tower1,wieze_format & tower2,wieze_format & tower3,wieze_format & tower4,wieze_format & tower5)
{
    string bufor[300];
    fstream plik;
    plik.open("save.txt");
    for(int i=0;i<300;i++)getline(plik,bufor[i]);

    graczyk.licznik_furii=stringoint(bufor[0]);
    graczyk.licznik_many=stringoint(bufor[1]);
    string help;
    help=bufor[2];
    if(help=="1000")
    {
        graczyk.wstep[0]=true;
        graczyk.wstep[1]=false;
        graczyk.wstep[2]=false;
        graczyk.wstep[3]=false;
    }
    if(help=="1100")
    {
        graczyk.wstep[0]=true;
        graczyk.wstep[1]=true;
        graczyk.wstep[2]=false;
        graczyk.wstep[3]=false;
    }

    if(help=="1110")
    {
        graczyk.wstep[0]=true;
        graczyk.wstep[1]=true;
        graczyk.wstep[2]=true;
        graczyk.wstep[3]=false;
    }

    if(help=="1111")
    {
        graczyk.wstep[0]=true;
        graczyk.wstep[1]=true;
        graczyk.wstep[2]=true;
        graczyk.wstep[3]=true;
    }

    graczyk.hp=stringoint(bufor[3]);
    graczyk.hp_max=stringoint(bufor[4]);
    graczyk.exp=stringoint(bufor[5]);
    graczyk.level=stringoint(bufor[6]);
    graczyk.sila=stringoint(bufor[7]);
    graczyk.sila/=1000;
    graczyk.def=stringoint(bufor[8]);
    graczyk.def/=1000;
    graczyk.piniondz=stringoint(bufor[9]);
    graczyk.ptk_nauki=stringoint(bufor[10]);

        if(bufor[11]=="4mm")        graczyk.pocisk_wlasny=mm20;
        if(bufor[11]=="120mm")      graczyk.pocisk_wlasny=mm120;
        if(bufor[11]=="fireball")   graczyk.pocisk_wlasny=mm130;


    for(int i=0;i<5;i++)
    {

        wieze_format tower;
    for(int i2=0;i2<20;i2++)if(wiezowce[i2].nazwa==bufor[12+(i*5)])tower=wiezowce[i2];
    if(bufor[12+(i*5)]!="")graczyk.l_wiez++;
    tower.reload=stringoint(bufor[13+(i*5)]);
    if(bufor[14+(i*5)]=="4mm")tower.pocisk_rama=mm20;
    if(bufor[14+(i*5)]=="120mm")tower.pocisk_rama=mm120;
    if(bufor[14+(i*5)]=="fireball")tower.pocisk_rama=mm130;
     tower.reload=stringoint(bufor[15+(i*5)]);
     tower.reload/=10;
     tower.pozycja=stringoint(bufor[16+(i*5)]);
        if(i==0)tower1=tower;
        else if(i==1)tower2=tower;
        else if(i==2)tower3=tower;
        else if(i==3)tower4=tower;
        else if(i==4)tower5=tower;

    }

    for(int i=0;i<30;i++)
    {
        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[37+(i*3)])graczyk.ekwipunek[i]=artefakty[i2];

        graczyk.ekwipunek[i].poziom=stringoint(bufor[38+(i*3)]);
        graczyk.ekwipunek[i].bonus=stringoint(bufor[39+(i*3)]);
        graczyk.ekwipunek[i].bonus=graczyk.ekwipunek[i].bonus/1000;
        graczyk.ekwipunek[i].img.setPosition(87+78*(i%10),647+78*(i/10));
    }

     for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[127]) graczyk.bron_ID=artefakty[i2];
        graczyk.bron_ID.poziom=stringoint(bufor[128]);
        graczyk.bron_ID.bonus=stringoint(bufor[129]);
        graczyk.bron_ID.bonus/=1000;

        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[130])graczyk.zbroja=artefakty[i2];
        graczyk.zbroja.poziom=stringoint(bufor[131]);
        graczyk.zbroja.bonus=stringoint(bufor[132]);
        graczyk.zbroja.bonus/=1000;

        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[133])graczyk.szyja=artefakty[i2];
        graczyk.szyja.poziom=stringoint(bufor[134]);
        graczyk.szyja.bonus=stringoint(bufor[135]);
        graczyk.szyja.bonus/=1000;

        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[136])graczyk.branza=artefakty[i2];
        graczyk.branza.poziom=stringoint(bufor[137]);
        graczyk.branza.bonus=stringoint(bufor[138]);
        graczyk.branza.bonus/=1000;

        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[139])graczyk.buty=artefakty[i2];
        graczyk.buty.poziom=stringoint(bufor[140]);
        graczyk.buty.bonus=stringoint(bufor[141]);
        graczyk.buty.bonus/=1000;

        for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[142])graczyk.ammo_typ=artefakty[i2];
        graczyk.ammo_typ.poziom=stringoint(bufor[143]);
       graczyk.ammo_typ.bonus=stringoint(bufor[144]);
       graczyk.ammo_typ.bonus/=1000;

                      graczyk.zbroja.img.setPosition(969,668);
                      graczyk.branza.img.setPosition(1271,668);
                      graczyk.bron_ID.img.setPosition(969,748);
                      graczyk.buty.img.setPosition(969,824);
                      graczyk.szyja.img.setPosition(1271,748);
                      graczyk.ammo_typ.img.setPosition(1271,824);


        for(int i=0;i<6;i++)
        {
              for(int i2=0;i2<100;i2++)if(artefakty[i2].nazwa==bufor[145+i])graczyk.szybkie[i]=artefakty[i2];
                int xx=592+(77*(i%3));
                int yy=1017+(76*(i/3));
                graczyk.szybkie[i].img.setPosition(xx,yy);
        }

}
