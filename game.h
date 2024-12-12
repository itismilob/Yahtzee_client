#pragma once
// �ΰ���
void gameRound();

void lock();

void combi();

void roll();


// ���� �Է�
void choice();

void clear();
// ���ھ� ���
void showScoreboard();
// ��Ģ ���
void showRules();

// ����
int main();

//�ֻ�������
int dice[5] = { 0 };
int locking[5] = { 0 };
int numbers[6] = { 0 };
int remain = 0;

// ���� �Է�
char says[100];

//���� ����
int combi_score = 0;
char* combiList[13] = {
    "One", "Two", "Three", "Four", "Five", "Six", "Three of Kinds", "Four of Kinds", "Full House", "Small Straight",
    "Large Straight", "Yahtzee", "Chance"
};
int isStop;

//���� ����
int score[13];
int total[MAX_USER] = { 0 };

void resetGame() {
    int i;
    for (i = 0;i < 5;i++) {
        dice[i] = 0;
        locking[i] = 0;
    }
    for (i = 0;i < 6;i++) {
        numbers[i] = 0;
    }
    remain = 0;

    //���� ����
    combi_score = 0;

    //���� ����
    for (i = 0;i < 13;i++) {
        score[i] = -1;
    }

    for (i = 0;i < MAX_USER;i++) { 
        total[i] = 0;
    }

}

//���尡 �����Ҷ� �۵��˴ϴ�.
void gameRound() {
    int i;
    for (i = 0; i < 6; i++) {
        numbers[i] = 0;
    }
    for (i = 0; i < 5; i++) {
        locking[i] = 0;
    }
    remain = 3;

    printf("�ֻ����� ������ ���� �ƹ� ���ڳ� �Է��ϼ���\n");
    choice();

    roll();
    lock();
}


//�ֻ����� �����ϴ�.
void roll() {
    int i;
    srand(time(NULL));
    for (i = 0; i < 5; i++) {
        if (locking[i] == 0) {
            dice[i] = rand() % 6 + 1;
        }
    }
    // �׽�Ʈ��
    /*for (i = 0; i < 5; i++) {
        if (locking[i] == 0) {
            dice[i] = 6;
        }
    }*/

    // ���⼭ ������ ������
    for (i = 0; i < 5; i++) {
        diceData.diceList[i] = dice[i];
    }
    diceData.isStop = FALSE;

    printf("send dice\n");
    sendDiceData(clientsock);

    remain = remain - 1;
}

//���� ��ų �� �ߵ��˴ϴ�.
void lock() {
    int i;
    isStop = FALSE;
    while (1) {
        showScoreboard();

        printf("\n\n�ֻ�����...\n");
        for (i = 0; i < 5; i++) {
            if (locking[i] == 1) {
                printf("[%d] ", dice[i]);
            }
            else {
                printf("%d ", dice[i]);
            }
        }

        // ���� �迭 �ʱ�ȭ
        for (i = 0; i < 6; i++) {
            numbers[i] = 0;
        }
        // �ֻ����� ���� �迭 �Է�
        for (i = 0; i < 5; i++) {
            numbers[dice[i] - 1]++;
        }

        combi_score = 0;
        if ((numbers[0] == 5 || numbers[1] == 5 || numbers[2] == 5 || numbers[3] == 5 || numbers[4] == 5 || numbers[5]
            ==
            5)) {
            if (score[11] != 50) {
                // ù ��° ���� Ȯ��
                printf("\nYahtzee!\n");
            }
            else {
                // �ι�° ���� Ȯ��
                printf("\nback-to-back Yahtzee bonus +50\n");
                combi_score += 50;
            }
        }

        printf("\n\n");

        if (remain > 0 && isStop == FALSE) {
            printf("���� ��Ű���ų� �����Ϸ��� �ֻ��� �ڸ���ȣ�� �Է��ϼ���.\n");
            printf("�ֻ��� �ڸ���ȣ�� ���ʺ��� 1���Դϴ�.\n");
            printf("\n");
            printf("���� Roll : %d\n", remain);
            printf("�ֻ����� �ٽ� ������ �ʹٸ� \"roll\"�� �Է��ϼ���.\n");
            printf("������ ä��� ���带 ������ �ʹٸ� \"stop\"�� �Է��ϼ���.\n");
            choice();

            if (!strcmp(says, "1") || !strcmp(says, "2") || !strcmp(says, "3") || !strcmp(says, "4") || !
                strcmp(says, "5")) {
                // lock�� ������ 1->0 0->1
                int temp = atoi(says) - 1;
                locking[temp] = locking[temp] == 1 ? 0 : 1;
            }
            else if (strcmp(says, "roll") == 0) {
                roll();
            }
            else if (strcmp(says, "stop") == 0) {
                isStop = TRUE;
            }
            else {
                printf("�߸� �Է��ϼ̽��ϴ�.\n");
            }
        }
        else {
            combi();
            if (isStop == TRUE) {
                for (i = 0; i < 5; i++) {
                    diceData.diceList[i] = dice[i];
                }
                diceData.isStop = TRUE;
                sendDiceData(clientsock);
                return;
            }
        }
    }
}


//���տ� ���� �����Դϴ�.
void combi() {
    int i;
    while (1) {
        printf("\n������ ��ȣ�� �Է��ϼ���.\n");
        if (remain > 0) {
            printf("���ư����� back�� �Է��ϼ���.\n");

        }

        int isCorrect = FALSE;


        int selected = -1;
        // �Է� �޴� �κ� �ݺ���
        while (1) {
            choice();
            if (strcmp(says, "back") == 0) {
                isStop = FALSE;
                return;
            }

            char* selectedList[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13" };
            for (i = 0; i < 13; i++) {
                if (strcmp(says, selectedList[i]) == 0) {
                    selected = i;
                    break;
                }
            }

            if (selected == -1) {
                printf("�߸� �Է��ϼ̽��ϴ�.\n");
            }
            else {
                break;
            }

        }

        // One ~ Six
        for (i = 0; i < 6; i++) {
            if (selected == i && score[i] == -1) {
                isCorrect = TRUE;
                combi_score = numbers[i] * (i + 1);
            }
        }

        // Three of kinds
        if (selected == 6 && score[6] == -1) {
            isCorrect = TRUE;
            if (numbers[0] >= 3 || numbers[1] >= 3 || numbers[2] >= 3 || numbers[3] >= 3 || numbers[4] >= 3 || numbers[5] >=3) {
                for (i = 0; i < 6; i++) {
                    combi_score += numbers[i] * (i + 1);
                }
            }
        }

        // Four of kinds
        if (selected == 7 && score[7] == -1) {
            isCorrect = TRUE;
            if (numbers[0] >= 4 || numbers[1] >= 4 || numbers[2] >= 4 || numbers[3] >= 4 || numbers[4] >= 4 || numbers[5] >=4) {
                for (i = 0; i < 6; i++) {
                    combi_score += numbers[i] * (i + 1);
                }
            }
        }

        // Full house
        if (selected == 8 && score[8] == -1) {
            isCorrect = TRUE;
            int dou = 0, tri = 0;
            for (i = 0; i < 6; i++) {
                if (numbers[i] == 2) {
                    dou++;
                }
                if (numbers[i] == 3) {
                    tri++;
                }
            }

            if (dou == 1 && tri == 1) {
                combi_score = 25;
            }
        }

        // Small straight
        if (selected == 9 && score[9] == -1) {
            isCorrect = TRUE;
            if ((numbers[0] >= 1 && numbers[1] >= 1 && numbers[2] >= 1 && numbers[3] >= 1) || (
                numbers[1] >= 1 && numbers[2] >= 1 && numbers[3] >= 1 && numbers[4] >= 1) || (
                    numbers[2] >= 1 && numbers[3] >= 1 && numbers[4] >= 1 && numbers[5] >= 1)) {
                combi_score = 30;
            }
        }
        // Large straight
        if (selected == 10 && score[10] == -1) {
            isCorrect = TRUE;
            if ((numbers[0] == 1 && numbers[1] == 1 && numbers[2] == 1 && numbers[3] == 1 && numbers[4] == 1) || (
                numbers[1] == 1 && numbers[2] == 1 && numbers[3] == 1 && numbers[4] == 1 && numbers[5] == 1)) {
                combi_score = 40;
            }
        }
        // Yahtzee
        if (selected == 11 && score[11] == -1) {
            isCorrect = TRUE;
            for (i = 0; i < 6; i++) {
                if (numbers[i] == 5) {
                    combi_score = 50;
                }
            }
        }

        // Chance
        if (selected == 12 && score[12] == -1) {
            isCorrect = TRUE;
            for (i = 0; i < 6; i++) {
                combi_score += numbers[i] * (i + 1);
            }
        }

        if (!isCorrect) {
            printf("�̹� ������ ���ֽ��ϴ�.\n");
            continue;
        }
        while (1) {
            printf("%s �� ������ %d�Դϴ�. ���� ������ yes, �ٽ� ���ư����� no\n", combiList[selected], combi_score);
            choice();

            if (strcmp(says, "yes") == 0) {
                score[selected] = combi_score;

                scoreData.combination = selected;
                scoreData.score = combi_score;
                isStop = TRUE;
                return;
            }
            if (strcmp(says, "no") != 0) {
                printf("�߸� �Է��߽��ϴ�.\n");
            }
        }
    }
}

// �����ǿ��� �Է� ������ �� 0�̸� �򰥸��ϱ� -�� ��ȯ
char* formatScore(int index) {
    int i;
    for (i = 0;i < MAX_USER;i++) {
        if (scoreboard.userScore[i][index] != -1) {
            printf("%3d", scoreboard.userScore[i][index]);
        }
        else {
            printf("%3s", "-");
        }

        if (i < MAX_USER - 1) printf(" | ");
    }
    printf("\n");
}


// �͹̳� ȭ�� �ʱ�ȭ
void clear() {
    for (int i = 0;i < 30;i++) {
        printf("\n");
    }
}

//�������Դϴ�.
void showScoreboard() {
    int i;

    printf("-Yahtzee--------------- ");
    for (i = 0;i < MAX_USER;i++) {
        printf("user %d", i+1);
        if (i < MAX_USER - 1) printf(" | ");
    }
    printf(" -\n");

    printf("[1] One                    "); formatScore(0);
    printf("[2] Two                    "); formatScore(1);
    printf("[3] Three                  "); formatScore(2);
    printf("[4] Four                   "); formatScore(3);
    printf("[5] Five                   "); formatScore(4);
    printf("[6] Six                    "); formatScore(5);
    printf("bonus 63 /                 "); formatScore(13);
    printf("--------------------------------------\n");
    printf("[7] Three of Kinds         "); formatScore(6);
    printf("[8] Four of Kinds          "); formatScore(7);
    printf("[9] Full House             "); formatScore(8);
    printf("[10] Small Straight        "); formatScore(9);
    printf("[11] Large Straight        "); formatScore(10);
    printf("[12] Yahtzee               "); formatScore(11);
    printf("[13] Chance                "); formatScore(12);
    printf("--------------------------------------\n");
    printf("Yahtzee bonus              "); formatScore(14);
    printf("Total score                "); formatScore(15);
    printf("--------------------------------------\n");
}

//�� �����Դϴ�.
void showRules() {
    clear();
    printf("�� ������ �ֻ��� 5���� �̿��Ͽ� ������ ����� �����Դϴ�.\n");
    printf("ó���� �Ѳ����� 5���� ���� ����, ���ϴ� �ֻ����� ŵ�� ��\n");
    printf("������ �ֻ����� ���� �� ������,\n");
    printf("�� ���忡 ��� ���� �� 3ȸ���� ���� �� �ֽ��ϴ�.\n");
    printf("�ֻ��� 3ȸ �ȿ� ������ ������ �ϳ� ������ ������ ���� �մϴ�.\n");
    printf("�ѹ� ������ ���� ���� �����δ� �ٽ� ������ ȹ���� �� �����ϴ�.\n\n");
    printf("���� ������ ���÷��� \"next\"�� �Է��ϼ���.\n");
    printf("���� �޴��� ���ư��÷��� �ƹ� ���ڳ� �Է��ϼ���.\n");
    choice();
    if (strcmp(says, "next") == 0) {
        clear();
        printf("------Yahtzee------\n");
        printf("One : �ֻ������� 1�� ���� �ֻ��� ���� x1�� ������ �մϴ�.\n");
        printf("Two : �ֻ������� 2�� ���� �ֻ��� ���� x2�� ������ �մϴ�.\n");
        printf("Three : �ֻ������� 3�� ���� �ֻ��� ���� x3�� ������ �մϴ�.\n");
        printf("Four : �ֻ������� 4�� ���� �ֻ��� ���� x4�� ������ �մϴ�.\n");
        printf("Five : �ֻ������� 5�� ���� �ֻ��� ���� x5�� ������ �մϴ�.\n");
        printf("Six : �ֻ������� 6�� ���� �ֻ��� ���� x6�� ������ �մϴ�.\n");
        printf("bonus : One ~ Six�� ������ ���� 63�� �̻��̶�� ���ʽ� 35���� ȹ���մϴ�\n");
        printf("-------------------\n");
        printf("Three of Kinds : ���� �ֻ����� 3�� �̻� �ִٸ� �ֻ��� ���� ������ ������ �մϴ�\n");
        printf("Four of Kinds : ���� �ֻ����� 4�� �̻� �ִٸ� �ֻ��� ���� ������ ������ �մϴ�\n");
        printf("Full House : �����ֻ��� 3�� + ���� �ֻ��� 2���� �ִٸ� 25���� ȹ���մϴ�\n");
        printf("ex : [3] [3] [3] [5] [5]\n");
        printf("Small Straight :  �ֻ��� ������ 4�� �̻��� ���ӵ� ���ڷ� \n");
        printf("�Ǿ��ִٸ� 30���� ȹ���մϴ�.\n");
        printf("ex : [1] [2] [3] [4] [6]\n");
        printf("Large Straight : �ֻ��� ���� ��ΰ� ���ӵ� ���ڷ�\n");
        printf("�Ǿ��ִٸ� 40���� ȹ���մϴ�.\n");
        printf("ex : [6] [5] [4] [3] [2]\n");
        printf("Yahtzee! : �ֻ��� ���ڰ� ��� ���ٸ� 50���� ȹ���մϴ�.\n");
        printf("�� ������ 50���� ä�� ���� �Ǵٽ� Yahtzee�� ��Ȳ�� �Ǵٽ� �´ٸ�\n");
        printf("� �������� ä��� 50���� �߰������� ȹ���մϴ�\n");
        printf("Chance : ������ �ֻ��� ���� ������ ������ ȹ���մϴ�\n\n\n");
        printf("���� �޴��� ���ư��÷��� �ƹ� ���ڳ� �Է��ϼ���.\n");
        choice();
    }
}


// ���� �Է� �޴°���
void choice() {
    memset(says, 0, 100);
    scanf("%s", &says);
}

