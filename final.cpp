#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <windows.h>

static int pi = 0; // player id counter
static int ti = 0; // team id counter

using namespace std;

struct Player
{
    int ID;
    char name[20];
    float price;
    int speed;
    int finishing;
    int defence;
    int team;
    char post[20];

} player[100];

struct Team
{
    int ID;
    char name[20];
    float money;
    int players[11];
    int win;
    int lose;
    int draw;
    int gf;
    int ga;
    int gd;
    int score;
} team[100];

// functions
void addPlayer();
void addTeam();
void buy();
void sell();
void friendlyMatch();
void Match(int, int);
int randWeather();
int winnerGoal();
int loserGoal(int);
void printtt(int, int, int);
void BeginLeague();
void printResult(int *, int);
void sort(int *, int);
void saveLeague(int *, int);
void saveTeams();
void loadTeams();

int main()
{
    // loadTeams();
    system("color 7");

    char amaliat[15], a2[10];
    while (1)
    {
        cin >> amaliat;

        if (!strcmp(amaliat, "new"))
        {
            cin >> a2;
            if (a2[0] == 'p')
                addPlayer();
            else if (a2[0] == 't')
                addTeam();
        }

        else if (!strcmp(amaliat, "buy"))
            buy();

        else if (!strcmp(amaliat, "sell"))
            sell();

        else if (!strcmp(amaliat, "friendlyMatch"))
            friendlyMatch();

        else if (!strcmp(amaliat, "Match"))
        {
            int tid1, tid2;
            scanf("%d %d", &tid1, &tid2);
            Match(tid1, tid2);
        }
        else if (!strcmp(amaliat, "BeginLeague"))
            BeginLeague();

        else if (!strcmp(amaliat, "end"))
        {
            // saveTeams();
            return 0;
        }
    }
}

// Add a player
void addPlayer()
{
    player[pi].ID = pi + 1;
    scanf("%s", &player[pi].name);
    scanf("%f", &player[pi].price);
    scanf("%d", &player[pi].speed);
    scanf("%d", &player[pi].finishing);
    scanf("%d", &player[pi].defence);
    player[pi].team = -1;

    if (player[pi].speed > player[pi].finishing)
    {
        if (player[pi].speed > player[pi].defence)
            strcpy(player[pi].post, "Midfielder");
        else
            strcpy(player[pi].post, "Defender");
    }

    else if (player[pi].finishing > player[pi].speed)
    {
        if (player[pi].finishing > player[pi].defence)
            strcpy(player[pi].post, "Striker");
        else
            strcpy(player[pi].post, "Defender");
    }

    else
        strcpy(player[pi].post, "Normal");

    pi++;
}

// Add a team
void addTeam()
{
    team[ti].ID = ti + 1;
    scanf("%s", &team[ti].name);

    // Check name
    for (int i = 0; i < ti - 1; i++)
        if (strcmp(team[ti].name, team[i].name) == 0)
        {
            cout << "this team name already exists" << endl;
            return;
        }

    scanf("%f", &team[ti].money);

    for (int i = 0; i < 11; i++)
        team[ti].players[i] = -1;

    team[ti].win = 0;
    team[ti].lose = 0;
    team[ti].draw = 0;
    team[ti].gf = 0;
    team[ti].ga = 0;
    team[ti].gd = 0;
    team[ti].score = 0;

    ti++;
}

// Buy a player
void buy()
{
    int pid, tid; // player id , team id
    scanf("%d %d", &pid, &tid);
    pid--;
    tid--;

    // find emty place
    int index;
    bool full = true;
    for (int i = 0; i < 11; i++)
        if (team[tid].players[i] == -1)
        {
            index = i;
            full = false;
            break;
        }

    if (pid >= pi)
        printf("player with the id %d doesnt exist\n", pid + 1);

    else if (tid >= ti)
        printf("team with the id %d doesnt exist\n", tid + 1);

    else if (player[pid].price > team[tid].money)
        cout << "the team cant afford to buy this player" << endl;

    else if (player[pid].team != -1)
        cout << "player already has a team" << endl;

    else if (full)
        cout << "team is full" << endl;

    else
    {
        team[tid].money -= player[pid].price;
        // add team for player
        player[pid].team = tid;
        // add player to team
        team[tid].players[index] = pid;
        printf("player %d added to the team %d successfully\n", pid + 1, tid + 1);
    }
}

// sell a player
void sell()
{
    int pid, tid; // player id , team id
    bool found = false;
    int index;
    scanf("%d %d", &pid, &tid);
    pid--;
    tid--;

    for (int i = 0; i < 11; i++)
        if (team[tid].players[i] == pid)
        {
            found = true;
            index = i;
            break;
        }

    if (tid >= ti)
        printf("team %d doesnt exist\n", tid + 1);

    else if (!found)
        cout << "team doesnt have this player" << endl;

    else
    {
        team[tid].money += player[pid].price;
        player[pid].team = -1;
        team[tid].players[index] = -1;
        printf("player %d removed from the team %d successfully\n", pid + 1, tid + 1);
    }
}

// friendlyMatch
void friendlyMatch()
{
    int tid1, tid2;
    bool emty = false;
    scanf("%d %d", &tid1, &tid2);
    tid1--;
    tid2--;

    for (int i = 0; i < 11; i++)
        if (team[tid1].players[i] == -1 || team[tid2].players[i] == -1)
        {
            emty = true;
            break;
        }

    if (tid1 >= ti || tid2 >= ti)
        cout << "team doesnt exist" << endl;

    else if (emty)
        cout << "the game can not be held due to loss of the players" << endl;

    else
    {
        int power1 = 0;
        int power2 = 0;

        for (int i = 0; i < 11; i++)
        {
            int in1 = team[tid1].players[i];
            int in2 = team[tid2].players[i];
            power2 += player[in2].speed + player[in2].finishing + player[in2].defence;
            power1 += player[in1].speed + player[in1].finishing + player[in1].defence;
        }

        if (power1 > power2)
            printf("team %d win\n", tid1 + 1);

        else if (power2 > power1)
            printf("team %d win\n", tid2 + 1);

        else
            printf("the match is draw\n");
    }
}

// Match
void Match(int tid1, int tid2)
{
    tid1--;
    tid2--;

    // empty place
    bool empty = false;
    for (int i = 0; i < 11; i++)
        if (team[tid1].players[i] == -1 || team[tid2].players[i] == -1)
        {
            empty = true;
            break;
        }

    // invalid team id
    if (tid1 >= ti || tid2 >= ti)
        cout << "team doesnt exist" << endl;

    // emty place
    else if (empty)
        cout << "the game can not be held due to loss of the players" << endl;

    // Match
    else
    {
        // Temp team
        Team tempt1 = team[tid1];
        Team tempt2 = team[tid2];

        // Weather
        int weather = randWeather();

        switch (weather)
        {
        case 0: // cloudy
            break;
        case 1: // snowy
            for (int i = 0; i < 11; i++)
            {
                int index = tempt1.players[i];
                player[index].defence *= -1;
                int index2 = tempt2.players[i];
                player[index2].defence *= -1;
            }
            break;
        case 2: // sunny
            for (int i = 0; i < 11; i++)
            {
                int index = tempt1.players[i];
                player[index].finishing -= 10;
                int index2 = tempt2.players[i];
                player[index2].finishing -= 10;
            }
            break;
        case 3: // rainy
            for (int i = 0; i < 11; i++)
            {
                int index = tempt1.players[i];
                player[index].speed /= 2;
                int index2 = tempt2.players[i];
                player[index2].speed /= 2;
            }
            break;
        }

        // Powers
        int power1 = 0, power2 = 0;
        for (int i = 0; i < 11; i++)
        {
            int in1 = tempt1.players[i];
            int in2 = tempt2.players[i];
            power1 += player[in1].speed + player[in1].finishing + player[in1].defence;
            power2 += player[in2].speed + player[in2].finishing + player[in2].defence;
        }

        // Goals
        int winnergoal = winnerGoal();
        int losergoal = loserGoal(winnergoal);

        // Printtt
        printtt(tid1, tid2, weather);
        system("color 7");

        // Result
        if (power1 > power2)
        {
            printf("team %d won\n\n", tid1 + 1);
            printf("WINNER TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid1 + 1, team[tid1].name, winnergoal, losergoal);
            printf("LOSER TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid2 + 1, team[tid2].name, losergoal, winnergoal);

            team[tid1].score += 3;

            team[tid1].win++;
            team[tid2].lose++;

            team[tid1].gf += winnergoal;
            team[tid1].ga += losergoal;
            team[tid1].gd += winnergoal - losergoal;

            team[tid2].gf += losergoal;
            team[tid2].ga += winnergoal;
            team[tid2].gd += losergoal - winnergoal;
        }
        else if (power2 > power1)
        {
            printf("team %d won\n\n", tid2 + 1);
            printf("WINNER TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid2 + 1, team[tid2].name, winnergoal, losergoal);
            printf("LOSER TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid1 + 1, team[tid1].name, losergoal, winnergoal);

            team[tid2].score += 3;

            team[tid2].win++;
            team[tid1].lose++;

            team[tid2].gf += winnergoal;
            team[tid2].ga += losergoal;
            team[tid2].gd += winnergoal - losergoal;

            team[tid1].gf += losergoal;
            team[tid1].ga += winnergoal;
            team[tid1].gd += losergoal - winnergoal;
        }
        else
        {
            printf("the match is draw\n\n");
            printf("TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid1 + 1, team[tid1].name, winnergoal, winnergoal);
            printf("TEAM %d : %s -> %d GOAL FOR / %d GOAL AGAINST\n", tid2 + 1, team[tid2].name, winnergoal, winnergoal);

            team[tid1].score += 1;
            team[tid2].score += 1;

            team[tid1].draw++;
            team[tid2].draw++;

            team[tid1].gf += winnergoal;
            team[tid1].ga += losergoal;

            team[tid2].gf += losergoal;
            team[tid2].ga += winnergoal;
        }
    }
}

// Random weather
int randWeather()
{
    static int runcount1 = 0;
    runcount1++;
    if (runcount1 == 1)
        srand(time(0));
    int w = rand() % 4; // 0.cloudy  1.snowy  2.sunny  3.rainy
    return w;
}

// Random goal -> winner
int winnerGoal()
{
    static int runcount2 = 0;
    runcount2++;
    if (runcount2 == 1)
        srand(time(0));

    int p = rand() % 100 + 1; // possibility

    if (p <= 50) // 1 - 50
        return 1;
    else if (p <= 80) // 50 - 80
        return 2;
    else if (p <= 95) // 80 - 95
        return 3;
    else if (p <= 100) // 95 - 100
    {
        srand(time(0));
        int winner = rand() % 10 + 4; // goals between 4 and 13
        return winner;
    }
}

// Rand goal -> loser
int loserGoal(int winner)
{
    int loser;
    static int runcount3 = 0;
    runcount3++;
    if (runcount3 == 1)
        srand(time(0));
    do
    {
        loser = rand();
    } while (loser >= winner);

    return loser;
}

// Print players, posts, air conditions
void printtt(int tid1, int tid2, int weather)
{
    system("color A");
    cout << "                            Match                              " << endl;
    cout << "_______________________________________________________________" << endl;
    cout << "|           TEAM 1           00:00          TEAM 2            |" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|_____                       . | .                       _____|" << endl;
    cout << "|     |                   .    |    .                   |     |" << endl;
    cout << "|____ |                  .     |     .                  | ____|" << endl;
    cout << "|   | |                 .      |      .                 | |   |" << endl;
    cout << "|   |.|                 .      |      .                 |.|   |" << endl;
    cout << "|___| |                  .     |     .                  | |___|" << endl;
    cout << "|     |                   .    |    .                   |     |" << endl;
    cout << "|_____|                      . | .                      |_____|" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|                              |                              |" << endl;
    cout << "|______________________________|______________________________|" << endl;
    printf("\nFIRST TEAM : %s\n\n", team[tid1].name);
    cout
        << left
        << setw(15)
        << "|Number"
        << left
        << setw(15)
        << "|Name"
        << left
        << setw(15)
        << "|Post"
        << left
        << setw(15)
        << "|ID"
        << endl
        << "---------------------------------------------------"
        << endl;

    for (int j = 0; j < 11; j++)
    {
        int index = team[tid1].players[j];
        cout
            << left
            << setw(15)
            << j + 1
            << left
            << setw(15)
            << player[index].name
            << left
            << setw(15)
            << player[index].post
            << left
            << setw(15)
            << player[index].ID
            << endl
            << endl;
    }
    cout << "___________________________________________________";
    printf("\n\nSECOND TEAM : %s\n\n", team[tid2].name);
    cout
        << left
        << setw(15)
        << "|Number"
        << left
        << setw(15)
        << "|Name"
        << left
        << setw(15)
        << "|Post"
        << left
        << setw(15)
        << "|ID"
        << endl
        << "---------------------------------------------------"
        << endl;

    for (int j = 0; j < 11; j++)
    {
        int index = team[tid2].players[j];
        cout
            << left
            << setw(15)
            << j + 1
            << left
            << setw(15)
            << player[index].name
            << left
            << setw(15)
            << player[index].post
            << left
            << setw(15)
            << player[index].ID
            << endl
            << endl;
    }

    // Weather
    switch (weather)
    {
    case 0: // cloudy
        cout << "\nTHE WEATHER IS CLOUDY TODAY\n"
             << endl;
        break;
    case 1: // snowy
        cout << "\nTHE WEATHER IS SNOWY TODAY\n"
             << endl;
        break;
    case 2: // sunny
        cout << "\nTHE WEATHER IS SUNNY TODAY\n"
             << endl;
        break;
    case 3: // rainy
        cout << "\nTHE WEATHER IS RAINY TODAY\n"
             << endl;
        break;
    }

    // Result
}

// League
void BeginLeague()
{
    system("cls");

    // Ready teams
    bool empty = false;
    int fullTeam[100], count = 0;
    for (int i = 0; i < ti; i++)
    {
        for (int k = 0; k < 11; k++)
            if (team[i].players[k] == -1)
            {
                empty = true;
                break;
            }
        if (!empty)
            fullTeam[count++] = team[i].ID;
    }

    // League
    int week = 1;
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            printf("***WEEK %d***\n", week++);
            Match(fullTeam[i], fullTeam[j]);
            Sleep(6000);
            system("cls");
        }
    }

    // Result
    sort(fullTeam, count);
    printResult(fullTeam, count);
    saveLeague(fullTeam, count);
}

// Print score board
void printResult(int *t, int count)
{
    system("cls");
    cout
        << endl
        << left
        << setw(15)
        << "|Team Name"
        << left
        << setw(10)
        << "|Score"
        << left
        << setw(15)
        << "|Goal For"
        << left
        << setw(20)
        << "|Goal Against"
        << left
        << setw(10)
        << "|Win"
        << left
        << setw(10)
        << "|Draw"
        << left
        << setw(10)
        << "|Lose"
        << left
        << setw(20)
        << "|Goal Difference"
        << endl
        << "----------------------------------------------------------------------------------------------------------"
        << endl;

    for (int i = 0; i < count; i++)
    {
        cout
            << left
            << setw(15)
            << team[t[i] - 1].name
            << left
            << setw(10)
            << team[t[i] - 1].score
            << left
            << setw(15)
            << team[t[i] - 1].gf
            << left
            << setw(20)
            << team[t[i] - 1].ga
            << left
            << setw(10)
            << team[t[i] - 1].win
            << left
            << setw(10)
            << team[t[i] - 1].draw
            << left
            << setw(10)
            << team[t[i] - 1].lose
            << left
            << setw(20)
            << team[t[i] - 1].gd
            << endl
            << endl;
    }
}

// Sort teams
void sort(int *t, int count)
{
    for (int i = 0; i < count; ++i)
        for (int j = i + 1; j < count; ++j)
            if (team[t[i] - 1].score < team[t[j] - 1].score)
            {
                struct Team temp = team[t[i] - 1];
                team[t[i] - 1] = team[t[j] - 1];
                team[t[j] - 1] = temp;
            }
            else if (team[t[i] - 1].score == team[t[j] - 1].score)
                if (team[t[i] - 1].gd < team[t[j] - 1].gd)
                {
                    struct Team temp = team[t[i] - 1];
                    team[t[i] - 1] = team[t[j] - 1];
                    team[t[j] - 1] = temp;
                }
}

// Save league data in file
void saveLeague(int *t, int count)
{
    FILE *fptr = fopen("league.txt", "a");

    fprintf(fptr, "name        ");
    fprintf(fptr, "score     ");
    fprintf(fptr, "gf       ");
    fprintf(fptr, "ga       ");
    fprintf(fptr, "win      ");
    fprintf(fptr, "draw     ");
    fprintf(fptr, "lose     ");
    fprintf(fptr, "gd       ");
    fprintf(fptr, "\n---------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        fprintf(fptr, "%s         ", team[t[i] - 1].name);
        fprintf(fptr, "%d        ", team[t[i] - 1].score);
        fprintf(fptr, "%d        ", team[t[i] - 1].gf);
        fprintf(fptr, "%d        ", team[t[i] - 1].ga);
        fprintf(fptr, "%d        ", team[t[i] - 1].win);
        fprintf(fptr, "%d        ", team[t[i] - 1].draw);
        fprintf(fptr, "%d        ", team[t[i] - 1].lose);
        fprintf(fptr, "%d        \n", team[t[i] - 1].gd);
    }
    fprintf(fptr, "\n---------------------------------------------------------------------------\n");
    fclose(fptr);
}

// Save teams
void saveTeams()
{
    FILE *fptr1 = fopen("teams.txt", "wb");

    fwrite(team, ti * sizeof(struct Team), 1, fptr1);

    fclose(fptr1);
}

// Load data from file
void loadTeams()
{
    FILE *fptr2 = fopen("teams.txt", "rb");
    if (fptr2 != NULL)
    {
        while (fread(&team[ti], sizeof(struct Team), 1, fptr2))
        {
            fread(&team[ti++], sizeof(struct Team), 1, fptr2);
        }

        fclose(fptr2);
    }
}
