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
    int index; // print_floor �Ҷ�
    int left_count;
} Floor;

typedef struct park
{
    int flag;
    Floor floors[4];
} Park;

// ���α׷� �Լ�
Park load();
int save(Park *pk);
int rotate_print(Park *pk, int flag); //�ٸ� �� ���
void print_menu(Park *pk);            //���� �޴� ���
void print_floors(Park *pk);          //�� �� ���

// ������ �Լ�
int isParked(Floor *fl, int number);
int isReserve(Floor *fl, int number);
void init_floor(Floor *fl, int index);
void parkingProcess(Park *pk);
void caroutProcess(Park *pk);
void reserveProcess(Park *pk);
void print_floor(Floor *fl);
int search(Floor *fl, int number); //����Ǿ������� index, �ȵǾ������� -1
int parking(Floor *fl, int number,
            int index); // ���� �ϱ� //������ �� ������ -1 ���� index
int reserve(Floor *fl, int number,
            int index); // �ڸ� ���� // ���� -1 ���� index
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
        // �ε� ����.
        // ����ǥ���Ұ�
    }

    //�޴� ���
    print_menu(&pk);

    return 0;
}
// ���α׷� �Լ�
//��������
int save(Park *pk)
{
    //��������
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
        printf("\n1. ����\n");
        printf("2. ����\n");
        printf("3. �������� ���\n");
        printf("4. ������Ȳ ���\n");
        printf("0. ����\n");
        printf("> ��ȣ ����:");

        scanf("%d", &menu);

        if (menu == 0)
        {
            save(pk);
            break;
        }

        switch (menu)
        {

        case 1:
            parkingProcess(pk); // �����ϱ�
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
} //�� �� ���

void caroutProcess(Park *pk)
{
    int number = 0, index = 0, i = 0;
    printf("> ���� �Է�(���� 4�ڸ�)");
    scanf("%d", &number);

    for (i = 1; i < 4; i++)
    {
        int ret1 = isParked(&pk->floors[i], number);
        if (ret1 > 0)
        {
            int ret2 = carout(&pk->floors[i], ret1);

            if (ret2 > 0)
            {
                printf("@���� �Ǿ����ϴ�.\n");
            }
            return;
        }
    }

    printf("@�ش� ������ �����ϴ�\n");
}

void reserveProcess(Park *pk)
{
    int fl = 0, index = 0, number = 0, i = 0;

    printf("���� ��ȣ �Է�(���� : 0):");
    scanf("%d%d", &fl, &index);
    printf("\n");
    printf("������� ��ȣ �Է� : ");
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
            // ��� ������ �̹� �ٸ����� ����Ǿ���.
            printf("%d �� ������ %d���� �̹� ������ �Ǿ� �ֽ��ϴ�.\n", number, i);
            return;
        }
    }

    if (pk->floors[fl].area[index].resv == -1)
    {
        reserve(&pk->floors[fl], number, index);
    }
    else
    {
        printf("@ B%d�� %d���� %d�� ��ϵǾ� �ֽ��ϴ�!\n", fl, index,
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
} //�ٸ� �� ���
void parkingProcess(Park *pk)
{ //
    int number, index;
    int i = 0;
    printf("\n> ���� �Է�(���� 4�ڸ�)");
    scanf("%d", &number);

    // ���������ϰ�� ���� ���� �������.
    for (i = 1; i <= 3; i++)
    {
        int ret1 = isReserve(&pk->floors[i], number);
        if (ret1 > 0)
        {
            int ret2 = parking(&pk->floors[i], number, ret1);
            if (ret2 > 0)
            {
                printf("@�������� : B%d�� %d�� �����Ǿ����ϴ�.\n", i, ret2);
            }
            return;
        }
    }

    //���������� �ƴѰ��

    printf("@���� ���ɰ���:");
    for (i = 1; i <= 3; i++)
    {
        printf("B%d��[%d��] ", i, pk->floors[i].left_count);
    }
    printf("\n");
    printf("\n");
    // @ ���� ���� ����: B1��[7��] B2��[10��] B3��[10��]

    int cur = 1;
    // B1�� ���

    cur = rotate_print(pk, 1);

    while (1)
    {
        //    // ������ȣ �Է�(�ٸ��� 0, ��� -1):
        printf("������ȣ �Է�(�ٸ��� 0, ��� -1):");
        // num : ������ ��ġ
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
                printf("@���� �Ǿ����ϴ�.\n");
            }
            return;
        }
    }
}
// ������ �Լ�
void print_floor(Floor *fl)
{

    printf("[B%2d��]========================\n", fl->index);
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
} //������ ��Ȳ ���

int search(Floor *fl, int number)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].resv == number)
            return i;
    }
    return -1;
} //����,���� �Ǿ������� index, �ȵǾ������� -1

// ���� �ϱ� //������ �� ������ -1 ���� index
int parking(Floor *fl, int number, int index)
{
    // number: ����, index: ��, location: �� ������ ������ġ

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
} // �ڸ� ���� // ���� -1 ���� index

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
} //�����Ǿ������� index, �ȵǾ������� -1

//�ش����� number�� ������ �Ǿ��ִ��� Ȯ���ϴ� �Լ�
int isReserve(Floor *fl, int number)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        if (fl->area[i].resv == number)
            return i; // ������ �Ǿ���
    }
    return -1;
} //����Ǿ������� index, �ȵǾ������� -1

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