#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <conio.h>
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
Texture zbrojowa,nadbroniowa,szyjowa,butna,renczna;
int Szerokosc=1600,Wysokosc=1200;
int level_limit[100];
int nr_wiezy_przerabianej=0;
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
        cout<<"Najechanie;  "<<xx<<","<<yy<<endl;
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
    int x,y;
    Sprite pocisk;
    bool moment;
    int speed;
    bool wlocie=true;
    float sila;
};

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
        nalozone.setTextureRect(IntRect(kolejny*200,0,200,200));
        nalozone.setOrigin(101,188);
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
    int x=xy.x;
    int y=xy.y;
    pocisk.x=x;
    pocisk.y=y;
    if(y>600)pocisk.moment=false;
    else pocisk.moment=true;
    pocisk.pocisk.setPosition(x,y);
    pocisk.pocisk.setRotation(90);

}


};

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
    bool furia=false;
    bool atak_miecza=false;
    bool atak_pistol=true;
    int zmienna_miny=0;
    bool wiecej_mocy;           ///true jesli dostalo buffa
    bool atak_mina=false;
    bool zyje=true;
    string wieze[6];
    int l_wiez=0;
    int pomocnik[2];
    int hp_max;
    int hp;
    int exp;
    int level;
    float sila;
    float def;
    int piniondz;
    int ptk_nauki;
    artefakty_format ekwipunek[30];
    artefakty_format bron_ID;
    artefakty_format zbroja;
    artefakty_format szyja;
    artefakty_format branza;
    artefakty_format buty;
    artefakty_format szybkie[6];


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
    void a_sie_wkurwilem()
    {
                obraz[2].setColor(Color(255,0,0));
                furia=true;
    }
    void dobra_spokoj()
    {
        obraz[2].setColor(Color(255,255,255));
        furia=false;
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
        else zbroja.nalozone.setPosition(x[2]+80,y[2]+60);

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
            zbroja.nalozone.setPosition(x[2]+80,y[2]+58);
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
            }
            if(typ=="zbroja"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=zbroja;

               usun_bonus(zbroja);
               zbroja.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,648+(pierwszy_wolny/3)*78);
            }
            if(typ=="szyja"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=szyja;

               usun_bonus(szyja);
               szyja.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,648+(pierwszy_wolny/3)*78);
            }
             if(typ=="branza"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=branza;

               usun_bonus(branza);
               branza.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,648+(pierwszy_wolny/3)*78);
            }
             if(typ=="buty"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=buty;

                usun_bonus(buty);
                buty.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,648+(pierwszy_wolny/3)*78);

            }
            if(typ=="bron"&&pierwszy_wolny>=0)
            {
                ekwipunek[pierwszy_wolny]=bron_ID;
                usun_bonus(bron_ID);
                bron_ID.umrzyj();
                ekwipunek[pierwszy_wolny].img.setPosition(87+(pierwszy_wolny%10)*78,648+(pierwszy_wolny/3)*78);

            }
    }

void sprawdz_level()
{
    for(int i=0;i<10;i++)if(exp-level_limit[level]>=0)
    {
    exp-=level_limit[level];
    level++;
    ptk_nauki+=5;
    }

}

};
class mobek_format
{
public:
    bool zyje=true;
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
    int hp;
    int exp;
    int piniondz;
    int nr_ataku=0;
    int ilosc_animacji_ataku=10;
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
        return false;
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

            }
            return zyje;
    }
      bool ujmij_zycie(gracz_format & gracz)
    {
        hp=hp-gracz.sila-(2*gracz.level)+gracz.bron_ID.bonus;
        img.setColor(Color(255,0,0));
        if(nazwa=="stopek")img.setColor(Color(0,0,0));
        if(hp<1)
        {
            zyje=false;
            gracz.piniondz+=piniondz;
            gracz.exp+=exp;

            }
            return zyje;
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
            }

            return zyje;
    }
    void atak(gracz_format & gracz)
    {
        if(nr_ataku%ilosc_animacji_ataku==0&&gracz.licznik_furii<=0)
            {
            gracz.hp=gracz.hp-(damage*(1-gracz.def));///MECHANIZM ODEJMOWANIA HP
            if(gracz.def>0.9)gracz.hp--;
            }
        nr_ataku++;

    }
    void zasieg()
    {

    }
};
class interfejs_format
{
    public:
    Texture tekstura;
    Texture tlo;
    Sprite obraz[4];
    Font czcionka;
    int ilosc_elementow=4;
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
        exp1.setPosition(1250,1002);
        exp2.setPosition(1250,1040);
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
    }
    void ustaw_staty(gracz_format & gracz)
    {
        exp1.setString(intostring(gracz.exp));
        exp2.setString(intostring(level_limit[gracz.level+1]));
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
    guziki_format guzik[10];
    Sprite tlo1,tlo2;
    Texture tlo_tekstura1,tlo_tekstura2;
    Text napisy[10];
    artefakty_format ulepszany;
    int koszty[10];
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
        for(int i =0;i<10;i++)napisy[i].setFont(czcionka);
        for(int i =0;i<10;i++)napisy[i].setCharacterSize(40);
        for(int i =0;i<10;i++)napisy[i].setColor(Color(50,50,50));
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
        napisy[3].setString(intostring(art.bonus));
        napisy[3].setPosition(70,480);

     }
     void ustaw_cene_ulepszenia(artefakty_format art)
     {
        napisy[4].setString("EXP:   "+intostring(art.cena*((art.poziom+1)*0.1)));
        napisy[4].setPosition(440,93);
        napisy[5].setString("HAJS:  "+intostring(art.cena*((art.poziom+1)*0.3)));
        napisy[5].setPosition(440,145);
        napisy[6].setString("PRZED:   "+intostring(art.bonus));
        napisy[6].setPosition(1100,93);
        napisy[7].setString("PO:      "+intostring(art.bonus*0.09+art.bonus));
        napisy[7].setPosition(1100,145);
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
        Sprite tlo;
        Texture tekstura_tla;
        int x=400,y=0;
        NPC_format handlarz[4];
        wieze_format wieze[8];
        gracz_format gracz;
        int ilosc_blokad;
       int blokadax[10][2];
       int blokaday[10][2];
       string teleport[4];
       string teleport_szczegol[4];
       int teleport_x[4][2];
       int teleport_y[4][2];

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
            for(int i=0;i<4;i++)
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
    short nr_fali;
    bool fala_start;
    int liczba_mobow;
    int oberwal(gracz_format & gracz)
    {

    }

};
///################################################################################################################################///
/// =======================================================         Funkcje niezależne ============================================///
///################################################################################################################################///

string komunikator(gracz_format &gracz, string komenda, wieze_format & wieza)
{
    if(komenda=="dodaj_wiezy_sile")
    {
        if(gracz.piniondz-1000>=0)wieza.sila+=0.25;

    }
   else if(komenda=="dodaj_wiezy_reload")
    {
        if(gracz.piniondz-1000>=0&wieza.reload-1>3)wieza.reload-=0.25;

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
                   funkcja.insert(0,komenda,0,3);
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
            gracz.speed++;
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
                gracz.ekwipunek[i].img.setPosition(87+78*(i%10),648+78*(i/10));
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
            return "KUPIONE";
         }
         else if(gracz.piniondz-p_hajs<0&&gracz.exp-p_exp<0)return "BRAK HAJSU I EXPA";
         else if(gracz.piniondz-p_hajs<0)return "BRAK HAJSU";
         else if(gracz.exp-p_exp<0)return "BRAK EXPA";




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
    if(mobek.nazwa=="stopek")
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
                else if(mobek.nr_anim==5)
        {
            mobek.y+=5;
        }
        else mobek.nr_anim=0;
    }
     if(mobek.nazwa=="ram")
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
RenderWindow Okno_Calkowite( VideoMode( Szerokosc, Wysokosc, 32 ), "Elektryk THE GAME",Style::Fullscreen);//Style::Fullscreen


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
artefakty[13].ustaw_artefakt(0,"Gowniany Pistol",0.5,"sila",2000,3,1,13,0);
artefakty[14].ustaw_artefakt(0,"Akacz",6000,"sila",4,4,1,14,1);
artefakty[15].ustaw_artefakt(5,"Klapaczos",600,"hp",600,5,1,15,0);
artefakty[16].ustaw_artefakt(5,"Kawa z termosu",600,"furia",6,6,1,16,1);
//3  4 1 1
/*
  if(rodzaj==0)bron=true;
   if(rodzaj==1)zbroja=true;
   if(rodzaj==2)szyja=true;
   if(rodzaj==3)branza=true;dw
   if(rodzaj==4)buty=true;
   if(rodzaj==5)potion=true;
   if(rodzaj==6)ammo=true; ///trzeba dołożyć
    */

///  ==================================           HANDLARZE                ==================================== ///

int dostepne_handle[64];                                /// Edycja dostepności handlu
for(int i=0;i<=16;i++)dostepne_handle[i]=i;                   ///i<ilosc artefaktóq
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
handlarz_poty.napisy[4].setPosition(630,620);
handlarz_poty.napisy[5].setPosition(630,690);
handlarz_poty.napisy[6].setPosition(700,770);
handlarz_poty.napisy[7].setPosition(475,840);
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

for(int i=0;i<6;i++)handlarz_poty.guzik[4+i]=guzik_plus;
handlarz_poty.guzik[6]=guzik_minus;
handlarz_poty.guzik[8]=guzik_minus;

handlarz_poty.guzik[4].x=754;
handlarz_poty.guzik[4].y=620;
handlarz_poty.guzik[4].komunikat="dodaj_wiezy_sile";

handlarz_poty.guzik[5].x=754;
handlarz_poty.guzik[5].y=690;
handlarz_poty.guzik[5].komunikat="dodaj_wiezy_reload";

handlarz_poty.guzik[6].x=625;
handlarz_poty.guzik[6].y=770;
handlarz_poty.guzik[6].komunikat="dodaj_wiezy_pozycje-";

handlarz_poty.guzik[7].x=789;
handlarz_poty.guzik[7].y=770;
handlarz_poty.guzik[7].komunikat="dodaj_wiezy_pozycje+";

handlarz_poty.guzik[8].x=308;
handlarz_poty.guzik[8].y=900;
handlarz_poty.guzik[8].komunikat="dodaj_wieze-";

handlarz_poty.guzik[9].x=400;
handlarz_poty.guzik[9].y=900;
handlarz_poty.guzik[9].komunikat="dodaj_wieze+";

for(int i=0;i<6;i++)handlarz_poty.guzik[4+i].ustaw_sie();

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
handlarz_zlom.guzik[0]=guzik_plus;
handlarz_zlom.guzik[0].x=800;
handlarz_zlom.guzik[0].y=110;
handlarz_zlom.guzik[0].ustaw_sie();
handlarz_zlom.guzik[0].komunikat="dodaj_wiedze";

handlarz_zlom.guzik[1]=guzik_plus;
handlarz_zlom.guzik[1].x=800;
handlarz_zlom.guzik[1].y=180;
handlarz_zlom.guzik[1].ustaw_sie();
handlarz_zlom.guzik[1].komunikat="dodaj_sile";

handlarz_zlom.guzik[2]=guzik_plus;
handlarz_zlom.guzik[2].x=800;
handlarz_zlom.guzik[2].y=250;
handlarz_zlom.guzik[2].ustaw_sie();
handlarz_zlom.guzik[2].komunikat="dodaj_obrone";

handlarz_zlom.guzik[3]=guzik_plus;
handlarz_zlom.guzik[3].x=800;
handlarz_zlom.guzik[3].y=320;
handlarz_zlom.guzik[3].ustaw_sie();
handlarz_zlom.guzik[3].komunikat="dodaj_szybkosc";

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
handlarz_kowal.x=1600;//2200;
handlarz_kowal.y=50;
handlarz_kowal.w=100;
handlarz_kowal.h=200;
handlarz_kowal.nazwa="Po prostu Kowal";
handlarz_kowal.anim1();
handlarz_kowal.kontrolix();
handlarz_kowal.obraz.setPosition(handlarz_kowal.x,handlarz_kowal.y);
handlarz_kowal.ustaw_czcionke(czcionka);
handlarz_kowal.napisy[0].setPosition(110,120);
handlarz_kowal.napisy[1].setPosition(110,180);
handlarz_kowal.napisy[2].setPosition(110,240);
handlarz_kowal.napisy[3].setPosition(110,300);
handlarz_kowal.guzik[0]=guzik_plus;
handlarz_kowal.guzik[0].x=800;
handlarz_kowal.guzik[0].y=110;
handlarz_kowal.guzik[0].ustaw_sie();
handlarz_kowal.guzik[0].komunikat="dodaj_wiedze";


handlarz_kowal.ilosc_guzikow=1;
for(int i=0;i<4;i++)handlarz_kowal.koszty[i]=500;
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
miecz.obraz.setTextureRect(IntRect(0,0,100,200));
miecz.obraz.setOrigin(-64+24,-31+173);

///  ==================================           Pociski               ==================================== ///
pocisk_format ammo[50];
pocisk_format mm20;
mm20.pocisk.setTexture(pociskraz);
mm20.pocisk.setTextureRect(IntRect(0,0,10,10));
mm20.speed=25;
mm20.sila=1.2;
Texture pociskdwa;
pociskdwa.loadFromFile("Grafika//EQ//pocisk2.png");
pocisk_format mm120;
mm120.pocisk.setTexture(pociskdwa);
mm120.pocisk.setTextureRect(IntRect(0,0,60,25));
mm120.speed=25;
mm120.sila=5;


///===================================          WIEŻE                       ======================================///



wieze_format wiezowce[60];
wiezowce[0].nazwa="vasadir";
wiezowce[0].ammo=0;
wiezowce[0].tekstura.loadFromFile("Grafika//postacie//wieza1.png");
wiezowce[0].obraz.setTexture(wiezowce[0].tekstura);
wiezowce[0].x=1700;
wiezowce[0].y=700;
wiezowce[0].w=100;
wiezowce[0].h=200;
wiezowce[0].kontrolix();
wiezowce[0].przesun(0,0);
wiezowce[0].pocisk.pocisk.setTexture(pociskraz);
wiezowce[0].pocisk_rama=mm120;
wiezowce[0].reload=7;
wiezowce[0].sila=2;
wiezowce[0].cena=9300;

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
wiezowce[1].cena=3300;


wiezowce[2].nazwa="rosiu";
wiezowce[2].ammo=0;
wiezowce[2].tekstura.loadFromFile("Grafika//postacie//wieza3.png");
wiezowce[2].obraz.setTexture(wiezowce[2].tekstura);
wiezowce[2].x=1900;
wiezowce[2].y=700;
wiezowce[2].w=100;
wiezowce[2].h=200;
wiezowce[2].kontrolix();
wiezowce[2].przesun(0,0);
wiezowce[2].pocisk_rama=mm120;
wiezowce[2].reload=11;
wiezowce[2].sila=20;
wiezowce[2].cena=6300;

///  ==================================           PIETRA                 ==================================== ///

korytarz_format pietro;
korytarz_format pietra[5];

pietra[1].tekstura_tla.loadFromFile("Grafika//tla//korytarz.png");
pietra[1].tlo.setTexture(pietra[1].tekstura_tla);

///blokady
pietra[1].ilosc_blokad=10;
pietra[1].ustaw_blokade(-2340,0,730,170,0);//rama
pietra[1].ustaw_blokade(-2340,850,730,900,1);
pietra[1].ustaw_blokade(730,0,760,900,2);
pietra[1].ustaw_blokade(-2400,0,-2300,900,3);
pietra[1].ustaw_blokade(-1840,460,-1220,900,4);
pietra[1].ustaw_blokade(-350,460,230,900,5);


///teleporty
pietra[1].ustaw_teleport(-220,170,-110,180,"sala","1",0);
pietra[1].ustaw_teleport(-1210,160,-1100,180,"sala","2",1);
pietra[1].ustaw_teleport(-1910,160,-1800,180,"sala","3",2);
pietra[1].ustaw_teleport(-2050,160,-1940,180,"sala","4",3);
///handlarze
pietra[1].handlarz[0]=handlarz_poty;
pietra[1].handlarz[1]=handlarz_zlom;
pietra[1].handlarz[2]=handlarz_kowal;


///wieże
pietra[1].wieze[0]=wiezowce[0];
pietra[1].wieze[1]=wiezowce[1];
pietra[1].wieze[2]=wiezowce[2];

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

moby[4].img.setTexture(dyskowa[0]);
moby[4].img.setScale(0.3,0.3);
moby[4].img.setOrigin(160,270);
moby[4].img.setTextureRect(IntRect(0,0,1200,1600));
moby[4].speed=7;
moby[4].range=20;
moby[4].w=360;
moby[4].h=480;
moby[4].nazwa="dysk";
moby[4].max_anim=14;
moby[4].hp=100;
moby[4].exp=10;
moby[4].piniondz=50;
moby[4].damage=20;
moby[4].ilosc_animacji_ataku=12;

moby[3].tekstura=zasilaczowa[0];
moby[3].img.setTexture(moby[3].tekstura);
moby[3].img.setTextureRect(IntRect(0,0,253,270));
moby[3].dol.setTexture(moby[3].tekstura);
moby[3].dol.setTextureRect(IntRect(10,260,100,70));
moby[3].img.setOrigin(126,270);
moby[3].dol.setOrigin(50,10);
moby[3].speed=7;
moby[3].range=20;
moby[3].w=253;
moby[3].h=270;
moby[3].nazwa="zasilacz";
moby[3].max_anim=14;
moby[3].hp=250;
moby[3].exp=40;
moby[3].piniondz=80;
moby[3].damage=20;



moby[2].tekstura.loadFromFile("Grafika//postacie//stop.png");
moby[2].img.setTexture(moby[2].tekstura);
moby[2].img.setTextureRect(IntRect(0,0,100,100));
moby[2].dol.setTexture(moby[2].tekstura);
moby[2].dol.setTextureRect(IntRect(0,100,100,100));
moby[2].dol.setOrigin(50,50);
moby[2].img.setOrigin(50,50);
moby[2].speed=10;
moby[2].w=100;
moby[2].h=100;
moby[2].nazwa="stopek";
moby[2].max_anim=6;
moby[2].hp=50;
moby[2].exp=20;
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
moby[1].hp=100;
moby[1].exp=10;
moby[1].piniondz=50;
moby[1].damage=20;
moby[1].ilosc_animacji_ataku=12;

for(int i=1;i<4;i++)moby[i].speed*=3;





///  ==================================           SALE                 ==================================== ///


sala_format sale[5][5];
sala_format sala;

Texture tekstura_sali;
tekstura_sali.loadFromFile("Grafika//tla//sala.png");
sale[1][1].falownik_nr.setPosition(Szerokosc/2,20);
sale[1][1].falownik_nr.setFont(czcionka);
sale[1][1].falownik_nr.setColor(Color(0,0,0));
sale[1][1].tlo.setTexture(tekstura_sali);




///  ==================================           GRACZ                ==================================== ///

gracz_format gracz1;
gracz_format gracz;
gracz1.bron=pistol;
gracz1.ilosc_czesci_ciala=5;
gracz1.speed=10;
gracz1.hp_max=400;
gracz1.hp=gracz1.hp_max;
gracz1.sila=100;
gracz1.def=0.01;
gracz1.piniondz=3000000;
gracz1.level=1;
gracz1.exp=30000;

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

gracz1.w=60;
gracz1.h=160;



gracz1.krok(700,500);

///===================================          INTERFEJS                      ======================================///
interfejs_format interfejs;
interfejs.czcionka=czcionka;
interfejs.tlo.loadFromFile("Grafika//menu//interfejs.png");
interfejs.tekstura.loadFromFile("Grafika//menu//interfejs2.png");
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
interfejs.ustaw_czcionke();
interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
interfejs.ustaw_teksty();

///  ==================================           ZMIENNE GLOBALNE              ==================================== ///
bool zmien_pietro=false;
int nr_pietra=1;
int nr_sala=0;
int z=0;
string poziom="korytarz";
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

wieze_format tower[6];
///  ==================================           Kursor              ==================================== ///
Texture kursorowa;
kursorowa.loadFromFile("Grafika//EQ//kursor.png");
Sprite kursor;
kursor.setTexture(kursorowa);
///  ==================================           Komunikat              ==================================== ///
string tresc_komunikat;
Text sam_komunikat;
sam_komunikat.setFont(czcionka);
sam_komunikat.setCharacterSize(40);
sam_komunikat.setPosition(50,50);
sam_komunikat.setColor(Color(30,30,30));
Texture komunikat_tekstura;
Sprite komunikat;
komunikat_tekstura.loadFromFile("Grafika//menu//komunikat.png");
komunikat.setTexture(komunikat_tekstura);
komunikat.setPosition(0,0);
bool wyswietl_komunikat=false;          ///Przy grzesiu
///IntRect(150, 0, 35, 100)
Clock clock;
int dziwny_licznik_zapoznawczy=0;
int licznik_klatek=0;
gracz=gracz1;
gracz.sprawdz_level();
/// Generator punktów levelu
for(int i=1;i<100;i++)level_limit[i]=500*i*i;
///  ==================================           PETLA GLOWNA              ==================================== ///
while( Okno_Calkowite.isOpen() )
    {

        Time t1 = clock.getElapsedTime();
        if(t1.asMilliseconds()>=1000)
        {
            clock.restart();

            }
        Event dzialanie;
            if(!pietro.zmiana_pietra)
            {

                pietro=pietra[nr_pietra];
                pietro.gracz=gracz1;
                pietro.zmiana_pietra=true;
                pietro.tlo.setPosition(pietro.x,pietro.y);
                 interfejs.naboje_max.setString(intostring(gracz.bron.magazyn));
            }


         if(poziom=="korytarz")
         {
bool komunikat_wlacz=false;

/// ######################################################################################################## ////
///  ==================================           KORYTARZ              ==================================== ////
/// ######################################################################################################## ////
    if( t1.asMilliseconds()%25==dziwny_licznik_zapoznawczy%25 )     ///ilosc klatek
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
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)Okno_Calkowite.close();

        }


        /// Warunki
        /// na okolice teleportu
         if(pietro.sprawdzno_teleport(gracz))
        {

        }
        if(pietro.sprawdz_wieze(gracz)>=0)
        {
                    int numeruj_wieze=pietro.sprawdz_wieze(gracz);
                  komunikat_wlacz=true;
                  string tekst_hmmm_komunikatu;
                  tekst_hmmm_komunikatu=pietro.wieze[numeruj_wieze].nazwa+" : "+intostring(pietro.wieze[numeruj_wieze].cena);
            sam_komunikat.setString(tekst_hmmm_komunikatu);
    }
        if(pietro.sprawdz_handlarzy(gracz)>=0)
        {
              komunikat_wlacz=true;
               sam_komunikat.setString(pietro.handlarz[pietro.sprawdz_handlarzy(gracz)].nazwa);
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
            if(gracz.zwrot_prawo)
            {
                gracz.odwroc(false);
            }
         gracz.animuj((licznik_klatek%48)/4);

                   }

            if( Keyboard::isKeyPressed( Keyboard::S ) &&poziom=="korytarz")
            {
                    przepustnik= pietro.sprawdz(gracz,2);
               if(przepustnik)
               {
                gracz.krok(0,gracz.speed);

                }
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
              if(!gracz.zwrot_prawo)
            {
                gracz.odwroc(true);
            }
              gracz.animuj((licznik_klatek%48)/4);
            }



           if( Keyboard::isKeyPressed( Keyboard::W ) &&poziom=="korytarz")
            {

                przepustnik= pietro.sprawdz(gracz,4);
               if(przepustnik)
               {
                gracz.krok(0,-gracz.speed);

                }
                 gracz.animuj((licznik_klatek%48)/4);

                  }

                if( Keyboard::isKeyPressed( Keyboard::E )&&poziom=="korytarz" )
            {


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
                gracz.wieze[gracz.l_wiez]=pietro.wieze[wieza_numeruj].nazwa;
                gracz.piniondz-=pietro.wieze[wieza_numeruj].cena;
                 for(int i2=0;i2<6;i2++)                  ///WSZYSTKIE WIEZE
                 {

                         for(int i=0;i<10;i++)if(gracz.wieze[i2]==wiezowce[i].nazwa)
                         {
                             tower[i2]=wiezowce[i];
                             i=11;

                         tower[i2].x=(i2%3)*350+400;
                         if(i2/3==0)tower[i2].y=73;
                         else tower[i2].y=900;
                         tower[i2].przesun(0,0);
                         }

                 }
                gracz.l_wiez++;
                pietro.wieze[wieza_numeruj].wziete=true;


                }

            }
            if(pietro.sprawdzno_teleport(gracz))
                {
                poziom="sala";
                poziom_szczegoly=pietro.sprawdz_teleport(poziom,gracz);
                zmien_pietro=true;


                }

            }


            ///MALOWANIE SPRAJTÓW
        Okno_Calkowite.clear( Color( 0, 0, 0 ) );

        interfejs.ustaw_staty(gracz);
        Okno_Calkowite.draw(pietro.tlo);
        for(int i=7;i>=0;i--)Okno_Calkowite.draw(pietro.wieze[i].obraz);                 ///Wiez jest 8
        for(int i=3;i>=0;i--)Okno_Calkowite.draw(pietro.handlarz[i].obraz);               /// Handlarzy jest 4
        Okno_Calkowite.draw(gracz.obraz[0]);
        Okno_Calkowite.draw(gracz.buty.nalozone);
        Okno_Calkowite.draw(gracz.obraz[1]);
        Okno_Calkowite.draw(gracz.obraz[2]);
        Okno_Calkowite.draw(gracz.zbroja.nalozone);
        Okno_Calkowite.draw(gracz.szyja.nalozone);
        Okno_Calkowite.draw(gracz.obraz[3]);
        Okno_Calkowite.draw(gracz.buty.nalozone);
        if(gracz.bron_ID.nazwa=="")Okno_Calkowite.draw(gracz.bron.obraz);
        Okno_Calkowite.draw(gracz.bron_ID.nalozone);
        Okno_Calkowite.draw(gracz.obraz[4]);
        Okno_Calkowite.draw(gracz.szyja.nalozone);
           /// Rysowanie kolejno glowy, tulowa, rak nog; odwrotnie!

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


///  ==================================           WULFSTAN               ==================================== ////

            if(handlarz.nazwa=="Mistrz Wulfstan")
            {
        if(zmien_pietro)
        {
            tower[nr_wiezy_przerabianej].obraz.setPosition(108,631);
            zmien_pietro=false;
            handlarz.napisy[4].setString(intostring(tower[nr_wiezy_przerabianej].sila*10));
            handlarz.napisy[5].setString(intostring(tower[nr_wiezy_przerabianej].reload));
            handlarz.napisy[6].setString(intostring(tower[nr_wiezy_przerabianej].x));
            handlarz.napisy[7].setString(tower[nr_wiezy_przerabianej].nazwa);


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
                    if(i>3)
                    {
                     i=10;
            komunikator(gracz,komunikat,tower[nr_wiezy_przerabianej]);
            handlarz.napisy[4].setString(intostring(tower[nr_wiezy_przerabianej].sila*10));
            handlarz.napisy[5].setString(intostring(tower[nr_wiezy_przerabianej].reload));
            handlarz.napisy[6].setString(intostring(tower[nr_wiezy_przerabianej].x+tower[nr_wiezy_przerabianej].pozycja));
            handlarz.napisy[7].setString(tower[nr_wiezy_przerabianej].nazwa);
             tower[nr_wiezy_przerabianej].obraz.setPosition(108,631);
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
                if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)
        {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";
            }
        }
        handlarz.napisy[0].setString("HP        :"+intostring(gracz.hp_max));
        handlarz.napisy[1].setString("SILA      :"+intostring(gracz.sila*10));
        handlarz.napisy[2].setString("OBRONA    :"+intostring(gracz.def*1000));
        handlarz.napisy[3].setString("SPEED     :"+intostring(gracz.speed));
        handlarz.napisy[8].setString("PTK NAUKI :"+intostring(gracz.ptk_nauki));
        handlarz.napisy[9].setString("KOSZTA    :"+intostring(gracz.level*500));
        interfejs.hajs.setString(intostring(gracz.piniondz));
                Okno_Calkowite.draw(handlarz.tlo1);
                Okno_Calkowite.draw(handlarz.tlo2);
                Okno_Calkowite.draw(handlarz.obraz);
                Okno_Calkowite.draw(tower[nr_wiezy_przerabianej].obraz);
            for(int i=0;i<handlarz.ilosc_guzikow;i++) Okno_Calkowite.draw(handlarz.guzik[i].obraz);
                ///Interfejs
                        for(int i=0;i<10;i++)Okno_Calkowite.draw(handlarz.napisy[i]);
                        for(int i=0;i<3;i++) Okno_Calkowite.draw(interfejs.obraz[i]);
                        Okno_Calkowite.draw(interfejs.naboje_max);
                        Okno_Calkowite.draw(interfejs.naboje_min);
        Okno_Calkowite.draw(interfejs.hajs);
        Okno_Calkowite.draw(interfejs.exp1);
        Okno_Calkowite.draw(interfejs.exp2);
        Okno_Calkowite.draw(interfejs.level);
        for(int i=0;i<6;i++) Okno_Calkowite.draw(gracz.szybkie[i].img);
                }
///  ==================================           Grzesio               ==================================== ////
///  ==================================           Handluje               ==================================== ////
                 if(handlarz.nazwa=="Handlarz Grzesio")
            {
            string przeslana_komenda;

            if(zmien_pietro)
            {

                for(int i=0;i<64;i++)
                {
                    artefakty[i].img.setPosition((i%16)*78+317,(i/16)*79+14);
                }
                zmien_pietro=false;
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
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&!wyswietl_komunikat)
        {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";

        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;

        if(!gracz.ekwipunek[aktualne_id].potion)
        {


        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");

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
         if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift&&kursor_y>=4)
         {
             int tymczasowe_ID=(((kursor_y-4)*10)+kursor_x);
             gracz.dodaj_bonus(gracz.ekwipunek[tymczasowe_ID]);
              artefakty_format przenosnia;
             if(gracz.ekwipunek[tymczasowe_ID].zbroja)
             {
                    przenosnia=gracz.zbroja;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.zbroja=gracz.ekwipunek[tymczasowe_ID];
                    gracz.zbroja.img.setPosition(969,668);
             }
             else  if(gracz.ekwipunek[tymczasowe_ID].branza)
             {
                    przenosnia=gracz.branza;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.branza=gracz.ekwipunek[tymczasowe_ID];
                    gracz.branza.img.setPosition(1271,668);

             }

             else  if(gracz.ekwipunek[tymczasowe_ID].bron)
             {
                    przenosnia=gracz.bron_ID;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.bron_ID=gracz.ekwipunek[tymczasowe_ID];
                    gracz.bron_ID.img.setPosition(969,748);

             }
             else  if(gracz.ekwipunek[tymczasowe_ID].buty)
             {
                    przenosnia=gracz.buty;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.buty=gracz.ekwipunek[tymczasowe_ID];
                    gracz.buty.img.setPosition(969,824);
            }

              else  if(gracz.ekwipunek[tymczasowe_ID].szyja)
            {
                    przenosnia=gracz.szyja;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*76);
                    gracz.szyja=gracz.ekwipunek[tymczasowe_ID];
                    gracz.szyja.img.setPosition(1271,748);
             }
             gracz.ekwipunek[tymczasowe_ID]=przenosnia;
             gracz.usun_bonus(przenosnia);


         }

        }

                interfejs.hajs.setString(intostring(gracz.piniondz));
                Okno_Calkowite.draw(handlarz.tlo1);
                Okno_Calkowite.draw(handlarz.tlo2);
                Okno_Calkowite.draw(handlarz.obraz);
                        for(int i=0;i<10;i++)Okno_Calkowite.draw(handlarz.napisy[i]);
                        for(int i=0;i<4;i++) Okno_Calkowite.draw(interfejs.obraz[i]);
                        for(int i=0;i<64;i++)Okno_Calkowite.draw(artefakty[dostepne_handle[i]].img);
                        for(int i=0;i<30;i++)Okno_Calkowite.draw(gracz.ekwipunek[i].img);

                        Okno_Calkowite.draw(gracz.zbroja.img);
                        Okno_Calkowite.draw(gracz.bron_ID.img);
                        Okno_Calkowite.draw(gracz.szyja.img);
                        Okno_Calkowite.draw(gracz.buty.img);
                        Okno_Calkowite.draw(gracz.branza.img);

                        Okno_Calkowite.draw(kursor);
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

                }

///  ==================================           Kowal                   ==================================== ////
///  ==================================           Ulepsza                 ==================================== ////
                 if(handlarz.nazwa=="Po prostu Kowal")
            {
            string przeslana_komenda;

            if(zmien_pietro)
            {

              kursor_y=4;
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
        if(dzialanie.type == Event::KeyReleased)

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
            przeslana_komenda=komunikator(gracz,"UPG"+intostring(aktualne_id));
        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.rozbierz("zbroja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.rozbierz("bron");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.rozbierz("buty");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.rozbierz("szyja");
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.rozbierz("branza");



        if(przeslana_komenda!="")
        {
            sam_komunikat.setString(przeslana_komenda);
            wyswietl_komunikat=1;
        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&!wyswietl_komunikat)
        {
                 poziom="korytarz";
                 zmien_pietro=true;
                poziom_szczegoly="1";

        }
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape)wyswietl_komunikat=0;
         if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::RShift)
         {
             int tymczasowe_ID=(((kursor_y%4)*10)+kursor_x);
             gracz.dodaj_bonus(gracz.ekwipunek[tymczasowe_ID]);
              artefakty_format przenosnia;
             if(gracz.ekwipunek[tymczasowe_ID].zbroja)
             {
                    przenosnia=gracz.zbroja;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.zbroja=gracz.ekwipunek[tymczasowe_ID];
                    gracz.zbroja.img.setPosition(969,668);
             }
             else  if(gracz.ekwipunek[tymczasowe_ID].branza)
             {
                    przenosnia=gracz.branza;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.branza=gracz.ekwipunek[tymczasowe_ID];
                    gracz.branza.img.setPosition(1271,668);

             }

             else  if(gracz.ekwipunek[tymczasowe_ID].bron)
             {
                    przenosnia=gracz.bron_ID;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.bron_ID=gracz.ekwipunek[tymczasowe_ID];
                    gracz.bron_ID.img.setPosition(969,748);

             }
             else  if(gracz.ekwipunek[tymczasowe_ID].buty)
             {
                    przenosnia=gracz.buty;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*78);
                    gracz.buty=gracz.ekwipunek[tymczasowe_ID];
                    gracz.buty.img.setPosition(969,824);
            }

              else  if(gracz.ekwipunek[tymczasowe_ID].szyja)
            {
                    przenosnia=gracz.szyja;
                    przenosnia.img.setPosition(87+kursor_x*78,648+(kursor_y-4)*76);
                    gracz.szyja=gracz.ekwipunek[tymczasowe_ID];
                    gracz.szyja.img.setPosition(1271,748);
             }

             gracz.ekwipunek[tymczasowe_ID]=przenosnia;
             gracz.usun_bonus(przenosnia);


         }

        }


                Okno_Calkowite.draw(handlarz.tlo1);
                Okno_Calkowite.draw(handlarz.tlo2);
                Okno_Calkowite.draw(handlarz.obraz);
                        for(int i=0;i<10;i++)Okno_Calkowite.draw(handlarz.napisy[i]);
                        for(int i=0;i<4;i++) Okno_Calkowite.draw(interfejs.obraz[i]);
                        for(int i=0;i<30;i++)Okno_Calkowite.draw(gracz.ekwipunek[i].img);

                        Okno_Calkowite.draw(gracz.zbroja.img);
                        Okno_Calkowite.draw(gracz.bron_ID.img);
                        Okno_Calkowite.draw(gracz.szyja.img);
                        Okno_Calkowite.draw(gracz.buty.img);
                        Okno_Calkowite.draw(gracz.branza.img);
                        Okno_Calkowite.draw(handlarz.ulepszany.img);
                        Okno_Calkowite.draw(kursor);
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

                }

                Okno_Calkowite.display();




           }

/// ######################################################################################################## ////
///  ==================================           SALE                  ==================================== ////
/// ######################################################################################################## ////
         if(licznik_klatek>1000000)licznik_klatek=0;
         else if(poziom=="sala")
         {

             if(zmien_pietro)
             {
                 sala.falownik_nr.setString(intostring(sala.nr_fali)+" STOP");
                 sala.falownik_nr.setColor(Color(0,255,0));
                 nr_sala=stringoint( poziom_szczegoly);
                 sala=sale[nr_pietra][nr_sala];
                 zmien_pietro=false;
                 gracz.krok(-300,500);
                 licznik_klatek=0;
                for(int i=0;i<31;i++)ammo[i]=mm20;
                for(int i=0;i<6;i++)tower[i].obraz.setPosition(tower[i].x+tower[i].pozycja,tower[i].y);
             }
            if( t1.asMilliseconds()%10==dziwny_licznik_zapoznawczy%10  )///ilosc klatek
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

            ///Jeżeli puszczony przycisk wraca do pozycji wyjsciowej

        if( dzialanie.type == Event::Closed )Okno_Calkowite.close();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Escape&&(!sala.fala_start||gracz.zmienna_mordu>100))
        {
            poziom="korytarz";
            zmien_pietro=true;
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
        }

        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::D)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::A)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F8)sala.nr_fali++;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::S)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::W)gracz.anim0();
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F5)Okno_Calkowite.close();

        }


            if(gracz.zyje){
          if( Keyboard::isKeyPressed( Keyboard::A ) &&poziom=="sala")
            {
                gracz.krok(-gracz.speed,0);
                gracz.animuj((licznik_klatek%48)/4);
                 if(gracz.zwrot_prawo)gracz.odwroc(false);


            }

             if( Keyboard::isKeyPressed( Keyboard::S ) &&poziom=="sala")
            {
                gracz.krok(0,gracz.speed);
                gracz.animuj((licznik_klatek%48)/4);


            }
             if( Keyboard::isKeyPressed( Keyboard::D ) &&poziom=="sala")
            {
              gracz.krok(gracz.speed,0);
                if(!gracz.zwrot_prawo)gracz.odwroc(true);
                gracz.animuj((licznik_klatek%48)/4);

            }

            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Return&&!sala.fala_start)
            {
                sala.fala_start=true;
                pseudo_fala=0;
                sala.falownik_nr.setString(intostring(sala.nr_fali)+" START");
                 sala.falownik_nr.setColor(Color(255,0,0));
            }
            if( Keyboard::isKeyPressed( Keyboard::W ) &&poziom=="sala")
            {
                gracz.krok(0,-gracz.speed);
                gracz.animuj((licznik_klatek%48)/4);

            }
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F1)gracz.zaloz_pistol();
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F2)gracz.zaloz_miecz();
            if(dzialanie.type == Event::KeyPressed&&dzialanie.key.code==Keyboard::F3&&gracz.furia)gracz.dobra_spokoj();
            else if(dzialanie.type == Event::KeyPressed&&dzialanie.key.code==Keyboard::F3&&!gracz.furia)gracz.a_sie_wkurwilem();
            if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F4&&!gracz.widac)gracz.widac=true;
            else if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::F4&&gracz.widac)gracz.widac=false;
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num1)gracz.uzyj(0);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num2)gracz.uzyj(1);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num3)gracz.uzyj(2);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num4)gracz.uzyj(3);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num5)gracz.uzyj(4);
        if(dzialanie.type == Event::KeyReleased&&dzialanie.key.code==Keyboard::Num6)gracz.uzyj(5);



              if( Keyboard::isKeyPressed( Keyboard::O )&&poziom=="sala"&&gracz.atak_pistol)
            {


           if(gracz.bron.load-1>=0&&(licznik_klatek%gracz.bron.odrzut==1)){
        gracz.widac=true;       ///Pocisk wyputy i wstawie wściekął morde
            ammo[gracz.bron.magazyn -gracz.bron.load].moment=gracz.zwrot_prawo;
            ammo[gracz.bron.magazyn -gracz.bron.load].x=gracz.x[1]+gracz.bron.x;
            ammo[gracz.bron.magazyn -gracz.bron.load].y=gracz.y[1]+gracz.bron.y-20;///Kalibracja lufy
            gracz.bron.load--;
            }
            interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
            }
             else if(gracz.l_wiez>0)
            {
                for(int i=0;i<100;i++)if(i==6){}
            }

           if( Keyboard::isKeyPressed( Keyboard::O )&&poziom=="sala"&&gracz.atak_miecza)
            {
            gracz.zmienna_miecza++;
            }


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
                for(int i=0;i<31;i++)ammo[i]=mm20;
                gracz.bron.reload=false;
                gracz.bron.load=gracz.bron.magazyn;
                interfejs.ustaw_ammo(gracz.bron.load,gracz.piniondz);
            }
        }

        if(gracz.hp<0)gracz.zyje=false;

         if(sala.fala_start)
        {
            if(licznik_klatek%20==1&&pseudo_fala>=5&&sala.liczba_mobow<=0)
                            {
                                sala.nr_fali++;
                                sala.fala_start=false;
                                pseudo_fala=0;
                                sala.liczba_mobow=0;
                                sala.falownik_nr.setString(intostring(sala.nr_fali)+" STOP");
                                sala.falownik_nr.setColor(Color(0,255,0));
                            }
              if(licznik_klatek%10==1&&sala.nr_fali<20)                                     ///GENERATOR FALI lol ale zjebany
              {

                     if(pseudo_fala<1)for(int i=0;i<50;i+=1+((7-(sala.nr_fali%7))))///10 na pseudo fali;
                        {
                            int nr_mobka=(i*23)%((sala.nr_fali/7)+3);
                            if(nr_mobka==0)nr_mobka=1+(sala.nr_fali/7);
                            if(nr_mobka>0&&(sala.nr_fali/7)+3>nr_mobka)
                            {
                            cout<<nr_mobka;
                            mobki[i]=moby[nr_mobka];
                            mobki[i].damage+=sala.nr_fali*2*(mobki[i].damage/10);           ///wzmocnienie hp i damage
                            mobki[i].hp+=sala.nr_fali*5*(mobki[i].hp/20);
                            mobki[i].nr_anim=i%mobki[i].max_anim;
                            mobki[i].x=Szerokosc+(i%3)*mobki[i].w/2+i*75;
                            mobki[i].y=350+(i%2)*mobki[i].h/2;
                            sala.liczba_mobow++;
                            cout<<sala.liczba_mobow<<endl;
                            }
                        }
            pseudo_fala++;

              }
              else
              {
                            int nr_mobka=4;
                            if(nr_mobka>0&&(sala.nr_fali/7)+3>nr_mobka)
                            {
                            mobki[10]=moby[nr_mobka];
                            mobki[10].nr_anim=2;
                            mobki[10].x=Szerokosc+mobki[10].w/2+100;
                            mobki[10].y=350+2*mobki[10].h/2;
                            }

              }

               if(licznik_klatek%8==1&&gracz.zyje)for(int i=0;i<50;i++)             ///Kwestie mobowe
               {
                   animacja(mobki[i]);
                   if(mobki[i].ruch(gracz)&&mobki[i].zyje)
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
                         for(int i2=0;i2<gracz.bron.magazyn -gracz.bron.load;i2++)if(ammo[i2].wlocie&&mobki[i].zyje)if(mobki[i].uderzony(ammo[i2]))

                         {
                            if(!mobki[i].ujmij_zycie(gracz,ammo[i2]))
                            {sala.liczba_mobow--;
                                cout<<sala.liczba_mobow<<endl;
                            }
                            ammo[i2].wlocie=false;


                         }
                    for(int i2=0;i2<gracz.l_wiez;i2++)if(tower[i2].pocisk.wlocie)if(mobki[i].uderzony(tower[i2].pocisk)&&mobki[i].zyje)   ///pociski wiezy
                    {
                            if(mobki[i].ujmij_zycie(gracz,tower[i2].pocisk,tower[i2]))sala.liczba_mobow--;
                            tower[i2].pocisk.wlocie=false;

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
                sala.falownik_nr.setString(intostring(gracz.zmienna_miecza));
                animacja_gracz(gracz,"jebs");
                gracz.zmienna_miecza++;
                if(gracz.zmienna_miecza>=90)
                    {
                for(int i=0;i<50;i++)if(mobki[i].ruch(gracz)&&mobki[i].zyje)mobki[i].ujmij_zycie(gracz);
                    gracz.zmienna_miecza=0;
                }
            }

                }

                if(!gracz.zyje)
                {
                    animacja_gracz(gracz,"zdech");

                }

        interfejs.ustaw_staty(gracz);
        interfejs.ustaw_zycie(gracz);
        sala.falownik_nr.setString(intostring(sala.nr_fali));
        Okno_Calkowite.clear( Color( 0, 0, 0 ) );
        Okno_Calkowite.draw(sala.tlo);
        for(int i=0;i<gracz.l_wiez;i++)
        {
            Okno_Calkowite.draw(tower[i].obraz);
            if(tower[i].pocisk.wlocie)Okno_Calkowite.draw(tower[i].pocisk.pocisk);
        }
        if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[0]);
         if(gracz.widac)Okno_Calkowite.draw(gracz.buty.nalozone);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[1]);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[2]);
         if(gracz.widac)Okno_Calkowite.draw(gracz.zbroja.nalozone);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[3]);
        if(gracz.bron_ID.nazwa==""||gracz.atak_miecza)Okno_Calkowite.draw(gracz.bron.obraz);
        if(!gracz.atak_miecza)Okno_Calkowite.draw(gracz.bron_ID.nalozone);
         if(gracz.widac)Okno_Calkowite.draw(gracz.obraz[4]);
        for(int i=0;i<gracz.bron.magazyn -gracz.bron.load;i++)
        {
            if(ammo[i].wlocie)Okno_Calkowite.draw(ammo[i].pocisk);
        }

         if(sala.fala_start)
        {
            for(int i=0;i<50;i++)if(mobki[i].zyje)Okno_Calkowite.draw(mobki[i].dol);
            for(int i=0;i<50;i++)if(mobki[i].zyje)Okno_Calkowite.draw(mobki[i].img);
        }

         for(int i=0;i<interfejs.ilosc_elementow;i++)Okno_Calkowite.draw(interfejs.obraz[i]);
        Okno_Calkowite.draw(interfejs.naboje_max);
        Okno_Calkowite.draw(interfejs.naboje_min);
        Okno_Calkowite.draw(interfejs.hajs);
        Okno_Calkowite.draw(interfejs.exp1);
        Okno_Calkowite.draw(interfejs.exp2);
        Okno_Calkowite.draw(interfejs.level);
        Okno_Calkowite.draw(sala.falownik_nr);
        for(int i=0;i<6;i++) Okno_Calkowite.draw(gracz.szybkie[i].img);

         if(gracz.zmienna_mordu>100)
                    {
                        gracz.zyje=true;
                        gracz.krok(700-gracz.x[2],200-gracz.y[2]);
                        komunikat.setScale(2,1);
                        Okno_Calkowite.draw(komunikat);
                        sam_komunikat.setString("    Jak mozna przegrac na "+intostring(sala.nr_fali)+" fali..");
                        Okno_Calkowite.draw(sam_komunikat);
                    }

        Okno_Calkowite.display();

         }

         }

    }


    return 0;
}



///Ustawienie teksturyy
/*
gracz1.tekstura[0].loadFromFile("Grafika//postacie//player2.png");
gracz1.tekstura[1].loadFromFile("Grafika//postacie//player3.png");

gracz1.obraz[0].setTextureRect(IntRect(0,0,55,95));
gracz1.obraz[0].setTexture(gracz1.tekstura[0]);                          ///noga
gracz1.obraz[0].setPosition(3,230);
gracz1.obraz[0].setOrigin(37,6);
gracz1.obraz[0].setRotation(-30);
gracz1.x[0]=45;
gracz1.y[0]=220;

gracz1.obraz[1].setTextureRect(IntRect(0,95,65,95));
gracz1.obraz[1].setTexture(gracz1.tekstura[0]);
gracz1.obraz[1].setPosition(-10,115);
gracz1.obraz[1].setOrigin(48,14);
gracz1.obraz[1].setRotation(-30);
gracz1.x[1]=50;
gracz1.y[1]=120;

gracz1.obraz[2].setTextureRect(IntRect(70,0,110,220));            ///tulow
gracz1.obraz[2].setTexture(gracz1.tekstura[0]);

gracz1.obraz[3].setTextureRect(IntRect(0,0,65,95));
gracz1.obraz[3].setTexture(gracz1.tekstura[0]);                          ///noga
gracz1.obraz[3].setPosition(3,230);
gracz1.obraz[3].setOrigin(37,6);
gracz1.obraz[3].setRotation(30);
gracz1.x[3]=45;
gracz1.y[3]=220;

gracz1.obraz[4].setTextureRect(IntRect(0,97,65,95));
gracz1.obraz[4].setTexture(gracz1.tekstura[0]);
gracz1.obraz[4].setPosition(-10,115);
gracz1.obraz[4].setOrigin(48,14);
gracz1.obraz[4].setRotation(-30);
gracz1.x[4]=50;
gracz1.y[4]=120;

for(int i3=0;i3<50;i3++)
                             {
                                 if(mobki[i3].zyje&&mobki[i3].nazwa!="")i3=60;
                                 else if(i3==49)
                                 {

                                     sala.nr_fali++;
                                     sala.fala_start=false;
                                     pseudo_fala=0;
                                     sala.falownik_nr.setString(intostring(sala.nr_fali)+" STOP");
                                        sala.falownik_nr.setColor(Color(0,255,0));

                                 }
                             }

*/
///Sterowanie
///
///F1 start fali
///
///
///
///gracz.img[2] zawsze bedzie głowa od niej liczony jest kraniec
/// Wieze mają reload i ammo
/// Reload to granica strzalu, ammo to licznik,

