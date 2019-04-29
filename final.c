#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "C:\\Users\\vgg\\Desktop\\juno\\mds\\10day\\final.bin"

typedef struct spot
{
    int resv;
    int real;
} Spot;

typedef struct floor
{
    Spot area[11];
    int index; // print_floor 할때
    int left_count;
} Floor;

typedef struct park
{
    int flag;
    Floor floors[4];
} Park;

// 프로그램 함수
Park load();
int save(Park *pk);
int rotate_print(Park *pk, int flag); //다른 층 출력
void print_menu(Park *pk);            //메인 메뉴 출력
void print_floors(Park *pk);          //각 층 출력

// 층관련 함수
int isParked(Floor *fl, int number);
int isReserve(Floor *fl, int number);
void init_floor(Floor *fl, int index);
void parkingProcess(Park *pk);
void caroutProcess(Park *pk);
void reserveProcess(Park *pk);
void print_floor(Floor *fl);
int search(Floor *fl, int number); //예약되어있으면 index, 안되어있으면 -1
int parking(Floor *fl, int number,
            int index); // 주차 하기 //주차할 수 없으면 -1 성공 index
int reserve(Floor *fl, int number,
            int index); // 자리 예약 // 실패 -1 성공 index
int carout(Floor *fl, int index);

int main(void)
{
    Park pk;

    int i = 0;
    for (i = 1; i < 4; i++)
    {
        init_floor(&pk.floors[i], i);
    }

    Park ret = load();
    if (ret.flag > 0)
    {
        pk = ret;
    }
    else
    {
        // 로드 실패.
        // 에러표시할것
    }

    //메뉴 출력
    print_menu(&pk);

    return 0;
}
// 프로그램 함수
//파일저장
int save(Park *pk)
{
    //파일저장
    FILE *fp = fopen(FILENAME, "w+");
    if (fp == 0)
        return -1;

    int size = sizeof(Park);
    int n = fwrite(pk, size, 1, fp);

    if (n == 1)
    {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return -1;
}

Park load()
{
    Park ret = {0};
    FILE *fp = fopen(FILENAME, "r");
    ret.flag = -1;
    if (fp == NULL)
    {

        return ret;
    }
    int size = sizeof(Park);
    int n = fread(&ret, size, 1, fp);
    if (n == 1)
    {
        ret.flag = 1;
        return ret;
    }

    return ret;
}

void print_menu(Park *pk)
{
    int menu = 0;
    while (1)
    {
        printf("\n1. 주차\n");
        printf("2. 출차\n");
        printf("3. 지정차량 등록\n");
        printf("4. 주차현황 출력\n");
        printf("0. 종료\n");
        printf("> 번호 선택:");

        scanf("%d", &menu);

        if (menu == 0)
        {
            save(pk);
            break;
        }

        switch (menu)
        {

        case 1:
            parkingProcess(pk); // 주차하기
            break;
        case 2:
            caroutProcess(pk);
            break;
        case 3:
            reserveProcess(pk);
            break;
        case 4:
            print_floors(pk);
            break;
        }
    }
}

void print_floors(Park *pk)
{
    int i;
    for (i = 1; i <= 3; i++)
    {
        print_floor(&pk->floors[i]);
    }
} //각 층 출력

void caroutProcess(Park *pk)
{
    int number = 0, index = 0, i = 0;
    printf("> 차번 입력(숫자 4자리)");
    scanf("%d", &number);

    for (i = 1; i < 4; i++)
    {
        int ret1 = isParked(&pk->floors[i], number);
        if (ret1 > 0)
        {
            int ret2 = carout(&pk->floors[i], ret1);

            if (ret2 > 0)
            {
                printf("@출차 되었습니다.\n");
            }
            return;
        }
    }

    printf("@해당 차량이 없습니다\n");
}

void reserveProcess(Park *pk)
{
    int fl = 0, index = 0, number = 0, i = 0;

    printf("층과 번호 입력(종료 : 0):");
    scanf("%d%d", &fl, &index);
    printf("\n");
    printf("등록차량 번호 입력 : ");
    scanf("%d", &number);
    printf("\n");

    int ret;
    for (i = 1; i <= 3; i++)
    {
        if (i == fl)
            continue;
        ret = isReserve(&pk->floors[i], number);
        if (ret > 0)
        {
            // 등록 차량이 이미 다른곳에 예약되었음.
            printf("%d 번 차량이 %d층에 이미 예약이 되어 있습니다.\n", number, i);
            return;
        }
    }

    if (pk->floors[fl].area[index].resv == -1)
    {
        reserve(&pk->floors[fl], number, index);
    }
    else
    {
        printf("@ B%d층 %d번에 %d가 등록되어 있습니다!\n", fl, index,
               pk->floors[fl].area[index].resv);
    }
}

int rotate_print(Park *pk, int flag)
{
    static int index;
    static int di;
    static int ret;

    if (flag == 1)
    {
        index = 1;
        di = 1;
    }

    ret = index;

    print_floor(&pk->floors[index]);

    if (index == 3)
    {
        di = -1;
    }
    if (index == 1)
    {
        di = 1;
    }

    index += di;

    return ret;
} //다른 층 출력
void parkingProcess(Park *pk)
{ //
    int number, index;
    int i = 0;
    printf("\n> 차번 입력(숫자 4자리)");
    scanf("%d", &number);

    // 지정주차일경우 이후 로직 수행안함.
    for (i = 1; i <= 3; i++)
    {
        int ret1 = isReserve(&pk->floors[i], number);
        if (ret1 > 0)
        {
            int ret2 = parking(&pk->floors[i], number, ret1);
            if (ret2 > 0)
            {
                printf("@지정차량 : B%d층 %d번 주차되었습니다.\n", i, ret2);
            }
            return;
        }
    }

    //지정차량이 아닌경우

    printf("@주차 가능공간:");
    for (i = 1; i <= 3; i++)
    {
        printf("B%d층[%d대] ", i, pk->floors[i].left_count);
    }
    printf("\n");
    printf("\n");
    // @ 주차 가능 공간: B1층[7대] B2층[10대] B3층[10대]

    int cur = 1;
    // B1층 출력

    cur = rotate_print(pk, 1);

    while (1)
    {
        //    // 주차번호 입력(다른층 0, 취소 -1):
        printf("주차번호 입력(다른층 0, 취소 -1):");
        // num : 주차할 위치
        scanf("%d", &index);
        printf("\n");
        if (index == 0)
        {
            cur = rotate_print(pk, 0);
        }
        else if (index == -1)
        {
            break;
        }
        else
        {
            int ret = parking(&pk->floors[cur], number, index);
            if (ret > 0)
            {
                printf("@주차 되었습니다.\n");
            }
            return;
        }
    }
}
// 층관련 함수
void print_floor(Floor *fl)
{

    printf("[B%2d층]========================\n", fl->index);
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].resv == -1 || fl->area[i].real != i)
        {
            printf("[%4d] ", fl->area[i].real);
        }
        else
        {
            printf("[resv] ");
        }
        if (i == 5)
            printf("\n");
    }
    printf("\n");
    printf("\n");
} //현재층 현황 출력

int search(Floor *fl, int number)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].resv == number)
            return i;
    }
    return -1;
} //예약,주차 되어있으면 index, 안되어있으면 -1

// 주차 하기 //주차할 수 없으면 -1 성공 index
int parking(Floor *fl, int number, int index)
{
    // number: 차번, index: 층, location: 그 층에서 주차위치

    if (fl->area[index].real == index)
    {
        fl->area[index].real = number;
        fl->left_count -= 1;
        return index;
    }
    return -1;
}

int reserve(Floor *fl, int number, int index)
{
    if (fl->left_count == 0)
        return -1;
    if (fl->area[index].resv != -1)
        return -1;
    fl->area[index].resv = number;
    fl->left_count--;
    return 1;
} // 자리 예약 // 실패 -1 성공 index

int carout(Floor *fl, int index)
{
    if (fl->left_count == 10)
        return -1;
    fl->left_count++;
    fl->area[index].real = index;
    return 1;
}

int isParked(Floor *fl, int number)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].real == number)
            return i;
    }
    return -1;
} //주차되어있으면 index, 안되어있으면 -1

//해당층에 number가 예약이 되어있는지 확인하는 함수
int isReserve(Floor *fl, int number)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].resv == number)
            return i; // 예약이 되어있
    }
    return -1;
} //예약되어있으면 index, 안되어있으면 -1

void init_floor(Floor *fl, int index)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        fl->area[i].real = i;
        fl->area[i].resv = -1;
    }
    fl->left_count = 10;
    fl->index = index;
}