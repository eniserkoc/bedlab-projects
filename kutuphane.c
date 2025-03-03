#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct kitap {
    char isim[50];
    char yazar[50];
    int sayfa;
    int durum;
    int id;
};
void kitapEkle(int i);
void kitapSil(int i);
void sorgula(int i); // kitap var mı kac tane var durumunu sorgula
void tumKitaplariListele();

struct kitap kitaplik[100];
int kitapSayisi = 0;
int main() {
    int a, id,b;
    puts("Kutuphaneye hos geldin.");
    while (1) {
        b=2;
        puts("Istediginiz islemi secmek icin ilgili numarayi giriniz.");
        puts("\tMENU\t");
        puts("1. Kitap Ekle\n2. Kitap Sil\n3. Kitap Sorgula\n4. Tum Kitaplari Listele\n5. Cikis");
        scanf("%d", &a);
        switch (a) {
            case 1:
                puts("Eklemek istediginiz kitabin ID'sini giriniz.");
                scanf("%d", &id);
                kitapEkle(id);
                puts("Baska bir islem? (Evet=1,Hayir=0)");
                while(b!=1 && b!=0){
                    scanf("%d",&b);
                    if(b==1){continue;}
                    else if(b==0){puts("Cikis yapiliyor...");
                        exit(0);}
                    else{puts("yanlis girdiniz...");}
                }
                break;
            case 2:
                puts("Silmek istediginiz kitabin ID'sini giriniz:");
                scanf("%d", &id);
                kitapSil(id);
                puts("Baska bir islem? (Evet=1,Hayir=0)");
                while(b!=1 && b!=0){
                    scanf("%d",&b);
                    if(b==1){continue;}
                    else if(b==0){puts("Cikis yapiliyor...");
                        exit(0);}
                    else{puts("yanlis girdiniz...");}
                }
                break;
            case 3:
                puts("Sorgulamak istediginiz kitabin ID'sini giriniz:");
                scanf("%d", &id);
                sorgula(id);
                puts("Baska bir islem? (Evet=1,Hayir=0)");
                while(b!=1 && b!=0){
                    scanf("%d",&b);
                    if(b==1){continue;}
                    else if(b==0){puts("Cikis yapiliyor...");
                        exit(0);}
                    else{puts("yanlis girdiniz...");}
                }
                break;
            case 4:
                tumKitaplariListele();
                puts("Baska bir islem? (Evet=1,Hayir=0)");
                while(b!=1 && b!=0){
                    scanf("%d",&b);
                    if(b==1){continue;}
                    else if(b==0){puts("Cikis yapiliyor...");
                        exit(0);}
                    else{puts("yanlis girdiniz...");}
                }
                break;
            case 5:
                puts("Cikis yapiliyor...");
                exit(0);
            default:
                puts("Gecersiz secim. Lutfen tekrar deneyiniz.");
        }
    }
}
void kitapEkle(int i){
    if (kitapSayisi >= 100) {
        puts("Kutuphane kapasitesi dolu.");
        return;
    }

    for (int x = 0; x < kitapSayisi; x++) {
        if (kitaplik[x].id == i) {
            puts("Bu ID ile zaten bir kitap var.");
            return;
        }
    }
    struct kitap new;
    new.id = i;
    printf("Kitap ismini giriniz: ");
    getchar();
    fgets(new.isim, 50, stdin);
    new.isim[strcspn(new.isim, "\n")] = '\0';
    printf("Yazar ismini giriniz: ");
    fgets(new.yazar, 50, stdin);
    new.yazar[strcspn(new.yazar, "\n")] = '\0';
    printf("Kitap sayfa sayisini giriniz: ");
    scanf("%d",&new.sayfa);
    new.durum = 1;
    kitaplik[kitapSayisi++] = new;
    puts("Kitap basariyla eklendi!");
}
void kitapSil(int i){
    for (int x = 0; x < kitapSayisi; x++) {
        if (kitaplik[x].id == i) {
            for (int j = x; j < kitapSayisi; j++) {
                kitaplik[j] = kitaplik[j + 1];
            }
            kitapSayisi--;
            puts("Kitap basariyla silindi!");
            return;
        }
    }
    puts("Bu ID ile eslesen bir kitap bulunamadi.");
}
void sorgula(int i){
    for (int x = 0; x < kitapSayisi; x++) {
        if (kitaplik[x].id == i) {
            printf("Kitap Bulundu:\n");
            printf("ID: %d\n", kitaplik[x].id);
            printf("Isim: %s\n", kitaplik[x].isim);
            printf("Yazar: %s\n", kitaplik[x].yazar);
            printf("Sayfa: %d\n", kitaplik[x].sayfa);
            printf("Durum: %s\n", kitaplik[x].durum == 1 ? "Mevcut" : "Odunc");
            return;
        }
    }
    puts("Bu ID ile eslesen bir kitap bulunamadi.");
}
void tumKitaplariListele() {
    if (kitapSayisi == 0) {
        puts("Kutuphane su anda bos.");
        return;
    }

    puts("Kutuphanedeki Kitaplar:");
    for (int i = 0; i < kitapSayisi; i++) {
        printf("ID: %d\n", kitaplik[i].id);
        printf("Isim: %s\n", kitaplik[i].isim);
        printf("Yazar: %s\n", kitaplik[i].yazar);
        printf("Sayfa: %d\n", kitaplik[i].sayfa);
        printf("Durum: %s\n", kitaplik[i].durum == 1 ? "Mevcut" : "Ödünç");
        puts("----------------------------");
    }
}
