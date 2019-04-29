#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "main.bin"

typedef struct spot {
	int resv;
	int real;
}Spot;

typedef struct floor {
	Spot area[11];
	int index;           // print_floor �Ҷ� 
	int left_count; 
}Floor;

typedef struct park {
	int flag; 
	Floor floors[4];
}Park;

int save(Park *pk);
Park load();

// ���α׷� �Լ�
void init_floor(Floor *fl, int index);
void print_menu(Park *pk); //���� �޴� ���
void print_floors(Park *pk); //�� �� ���
int rotate_print(Park *pk, int flag); //�ٸ� �� ���


// ������ �Լ�
int isParked(Floor *fl, int number);  // �ش� ���� number(������ȣ)�� �ٸ� ���� ������ �Ǿ�����?
int isReserve(Floor *fl, int number); // �ش� ���� number(������ȣ)�� �ٸ� ���� ���������� �Ͽ�����?
void parkingProcess(Park *pk); // ���� ����� �� ���� �Լ�(parking) ȣ��
void caroutProcess(Park *pk);  // ���� ����� �� ���� �Լ�(carout) ȣ��
void reserveProcess(Park *pk); // ���� ���� ��� ���� ����� �� ���� �Լ�(reserve) ȣ��
void print_floor(Floor *fl);   // �ش��� ���
int search(Floor *fl, int number); //����Ǿ������� index, �ȵǾ������� -1
int parking(Floor *fl, int number, int index); // ���� �ϱ� //������ �� ������ -1 ���� index
int reserve(Floor *fl, int number, int index); // �ڸ� ���� // ���� -1 ���� index
int carout(Floor *fl, int index); // ���� ��ġ�� �ʱⰪ���� ����


int main(void) {

	Park pk ;

	int i = 0;
	for (i = 1; i < 4; i++) {
		init_floor(&pk.floors[i], i);
	}

	Park ret = load();
	if (ret.flag >0) {
		pk = ret;
	}else {
		// �ε� ����.
		// ����ǥ���Ұ�
	}


	//�޴� ���
	print_menu(&pk);

	save(&pk);

	return 0;

}


//��������
int save(Park *pk) {
	//��������
	FILE *fp = fopen(FILENAME, "wb");
	if (fp == 0)
		return -1;

	int size = sizeof(Park);
	int n = fwrite(pk, size, 1, fp);

	if (n == 1) {
		fclose(fp);
		return 1;
	}
	fclose(fp);
	return -1;
}
//���� �ҷ�����
Park load() {
	Park ret = { 0 };
	FILE *fp = fopen(FILENAME, "rb");
	ret.flag = -1;
	if (fp == NULL) {

		return ret;
	}
	int size = sizeof(Park);
	int n = fread(&ret, size, 1, fp);
	if (n == 1) {
		ret.flag = 1;
		return ret;
	}

	return ret;
}

// ���α׷� �Լ�
void init_floor(Floor *fl, int index) {
	int i;
	for (i = 1; i <= 10; i++) {
		fl->area[i].real = i;
		fl->area[i].resv = -1;
	}
	fl->left_count = 10;
	fl->index = index;
}
void print_menu(Park *pk) {
	int menu = 0;
	while (1) {
		printf("1. ����\n");
		printf("2. ����\n");
		printf("3. �������� ���\n");
		printf("4. ������Ȳ ���\n");
		printf("0. ����\n");
		printf("> ��ȣ ����:");

		scanf("%d", &menu);

		if (menu == 0) {
			break;
		}

		switch (menu) {

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
void print_floors(Park *pk) {
	int i = 1;
	for (; i < 4; i++) {
		print_floor(&pk->floors[i]);
	}
} //�� �� ��� 
int rotate_print(Park *pk, int flag) {
	static int index;
	static int di;
	static int ret;

	if (flag == 1) {
		index = 1;
		di = 1;
	}
	ret = index;
	print_floor(&pk->floors[index]);
	if (index == 3)
		di = -1;
	if (index == 1)
		di = 1;
	index += di;
	return ret;
} //�ٸ� �� ���




// ������ �Լ�
//�����Ǿ������� index, �ȵǾ������� -1
int isParked(Floor *fl, int number) {
	int i;
	for (i = 1; i <= 10; i++) {
		if (fl->area[i].real == number)
			return i;
	}
	return -1;
}
//�ش����� number�� ������ �Ǿ��ִ��� Ȯ���ϴ� �Լ�
int isReserve(Floor *fl, int number) {
	int i;
	for (i = 1; i <= 10; i++) {
		if (fl->area[i].resv == number)
			return i; // ������ �Ǿ�����
	}
	return -1;
}
void parkingProcess(Park *pk) {// 
	int number = 0, index = 0;
	int i = 0;
	printf("\n> ���� �Է�(���� 4�ڸ�)");
	scanf("%d", &number);


	// ���������ϰ�� ���� ���� �������.
	for (i = 1; i < 4; i++) {
		int temp = isReserve(&pk->floors[i], number);
		if (temp > 0) {
			int ret = parking(&pk->floors[i], number, temp);
			if (ret > 0) {
				printf("@�������� : B%d�� %d�� �����Ǿ��ֽ��ϴ�!\n", i, temp);
			}
			return;
		}
	}




	printf("@���� ���ɰ���: ");
	for (i = 1; i < 4; i++) {
		printf("B%d�� [%2d��] ", i, pk->floors[i].left_count);
	}
	printf("\n");
	printf("\n");
	// @ ���� ���� ����: B1��[7��] B2��[10��] B3��[10��]




	int cur = 1;
	int num = 0; // ���� ��ġ
	int flag = 0;
	// B1�� ���
	cur = rotate_print(pk, 1);
	while (1) {
		// 	// ������ȣ �Է�(�ٸ��� 0, ��� -1):
		printf("������ȣ �Է�(�ٸ��� 0, ��� -1):");
		// num : ������ ��ġ
		scanf("%d", &num);
		if (num == 0) {
			cur = rotate_print(pk, 0);
		}
		else if (num == -1) {
			break;
		}
		else {

			flag = parking(&pk->floors[num], number, num);
			printf("@���� �Ǿ����ϴ�!\n");
			return;
		}
	}


}
void caroutProcess(Park *pk) {
	int number = 0, index = 0, i = 0;
	printf("> ���� �Է�(���� 4�ڸ�)");
	scanf("%d", &number);

	for (i = 1; i < 4; i++) {
		int parkRet = isParked(&pk->floors[i], number);
		if (parkRet > 0) {
			int tmp = carout(&pk->floors[i], parkRet);

			if (tmp > 0) {
				printf("@���� �Ǿ����ϴ�.\n");
			}
			return;
		}

	}

	printf("@�ش� ������ �����ϴ�\n");

}
void reserveProcess(Park *pk) {
	int fl = 0, num = 0, number = 0, i = 0;

	printf("���� ��ȣ �Է�(���� : 0):");
	scanf("%d%d", &fl, &num);
	printf("������� ��ȣ �Է� : ");
	scanf("%d", &number);


	if (pk->floors[fl].area[num].resv == -1) {
		reserve(&pk->floors[fl], number, num);
		printf("@����� �Ǿ����ϴ�.\n");
	}
	else {
		printf("@ B%d�� %d���� %d�� ��ϵǾ� �ֽ��ϴ�!\n"
			, fl, num, pk->floors[fl].area[num].resv);
	}

}
void print_floor(Floor *fl) {

	printf("[B%2d��]=================\n", fl->index);
	int i;
	for (i = 1; i <= 10; i++) {
		if (fl->area[i].resv == -1 || fl->area[i].real != i) {
			printf("[%4d] ", fl->area[i].real);
		}
		else {
			printf("[resv] ");
		}
		if (i == 5)
			printf("\n");
	}
	printf("\n");
	printf("\n");
} //������ ��Ȳ ���
int search(Floor *fl, int number) {
	int i;
	for (i = 1; i <= 10; i++) {
		if (fl->area[i].resv == number)
			return i;
	}
	return -1;
} //����,���� �Ǿ������� index, �ȵǾ������� -1

// ���� �ϱ� //������ �� ������ -1 ���� index
int parking(Floor *fl, int number, int index) {
	// number: ����, index: ��, location: �� ������ ������ġ
	
	if (fl->area[index].real == index) {
		fl->area[index].real = number;
		fl->left_count -= 1;
		return index;
	}

	return -1;
}
int reserve(Floor *fl, int number, int index) {
	if (fl->left_count == 0)
		return -1;
	if (fl->area[index].resv != -1 && fl->area[index].real != index)
		return -1;
	fl->area[index].resv = number;
	fl->left_count--;
	return 1;
} // �ڸ� ���� // ���� -1 ���� index
int carout(Floor *fl, int index) {
	if (fl->left_count == 10)
		return -1;
	fl->left_count++;
	fl->area[index].real = index;
	return 1;
}






