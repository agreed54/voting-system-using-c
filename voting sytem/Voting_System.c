#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Date
{
    int year;
    int month;
    int day;
};

struct Candidate
{
    char name[50];
    char party[50];
    char constituency[50];
    int votes;
};

struct Voter
{
    int sno;
    char name[50];
     struct {
        int year;
        int month;
        int day;
    } dob;
    char address[100];
    char password[20];
    int voted;
};

struct ElectionSchedule
{
    char constituency[50];
    struct Date date;
};

struct Voter voters[100];
struct Candidate candidates[50];
struct ElectionSchedule electionSchedule[4];

void adminMenu();
void voterMenu();
void registerVoter();
void updateVoterDetails();
void searchVoterDetails();
void castVote();
void viewVoteResult();
void createElectionSchedule();
void addCandidate();
void updateCandidate();
void deleteCandidate();

int main()
{
    struct Voter voters[100];
    struct Candidate candidates[50];
    struct ElectionSchedule electionSchedule[50];

    int numVoter = 0;
    int numCandidates;
    int numConstituencies;
    int choice;
    do
    {
        printf("\nMain Menu\n");
        printf("1. Admin\n");
        printf("2. Voter\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            adminMenu();
            break;
        case 2:
            voterMenu();
            break;
        case 3:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while(choice != 3);

    return 0;
}

void adminMenu()
{
    char username[7];
    char password[7];
    int choice;
    printf("Enter Username: ");
    scanf("%s",username);
    printf("Enter Password: ");
    scanf("%s",password);

    if (strcmp(username, "Admin") == 0 && strcmp(password, "admin") == 0)
    {
        while(choice!=5)
        {
            printf("\nAdmin Menu\n");
            printf("1. Create Election Schedule\n");
            printf("2. Add Candidate\n");
            printf("3. Update Candidate\n");
            printf("4. Delete Candidate\n");
            printf("5. View vote result\n");
            printf("6. Back to Main Menu\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch(choice)
            {
            case 1:
                createElectionSchedule();
                break;
            case 2:
                addCandidate();
                break;
            case 3:
                updateCandidate();
                break;
            case 4:
                deleteCandidate();
                break;
            case 5:
                viewVoteResult();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
    else
    {
        printf("\nInvalid username or password \n");
    }

}

void createElectionSchedule()
{
    int numConstituencies = 0;
    if (numConstituencies >= 4)
    {
        printf("Maximum number of constituencies reached. Cannot add more.\n");
        return;
    }

    struct ElectionSchedule newSchedule;

    printf("Enter Constituency: ");
    getchar();
    fgets(newSchedule.constituency, sizeof(newSchedule.constituency), stdin);
    strtok(newSchedule.constituency, "\n");

    printf("Enter Date of Election (YYYY MM DD): ");
    scanf("%d", &newSchedule.date.year);
    scanf("%d", &newSchedule.date.month);
    scanf("%d", &newSchedule.date.day);

    electionSchedule[numConstituencies] = newSchedule;
    numConstituencies++;

    FILE *file = fopen("schedule.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s\t%d%d%d\n", newSchedule.constituency, newSchedule.date.year, newSchedule.date.month, newSchedule.date.day);

    fclose(file);

    printf("Election schedule created successfully.\n");
}


void addCandidate()
{
    FILE *candidateFile = fopen("candidatelist.txt", "a");
    if (candidateFile == NULL)
    {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    struct Candidate newCandidate;

    printf("Enter Candidate Name: ");
    getchar();
    fgets(newCandidate.name, sizeof(newCandidate.name), stdin);
    strtok(newCandidate.name, "\n");
    printf("Enter Political Party: ");
    fgets(newCandidate.party, sizeof(newCandidate.party), stdin);
    strtok(newCandidate.party, "\n");
    printf("Enter Constituency: ");
    fgets(newCandidate.constituency, sizeof(newCandidate.constituency), stdin);
    strtok(newCandidate.constituency, "\n");
    newCandidate.votes = 0;

    fprintf(candidateFile, "%s %s %s %d\n", newCandidate.name, newCandidate.party, newCandidate.constituency, newCandidate.votes);

    fclose(candidateFile);

    printf("\n*** Candidate added successfully ***\n");
}

void updateCandidate()
{
    char name[50];
    printf("Enter the name of the candidate you want to update: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    strtok(name, "\n");

    FILE *file = fopen("candidatelist.txt", "r+");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct Candidate candidate;
    int found = 0;

    while (fscanf(file, "%s %s %s %d\n", candidate.name, candidate.party, candidate.constituency, &candidate.votes) != EOF)
    {
        if (strcmp(candidate.name, name) == 0)
        {
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Candidate with name %s not found.\n", name);
        fclose(file);
        return;
    }

    printf("Enter updated Candidate Name: ");
    fgets(candidate.name, sizeof(candidate.name), stdin);
    strtok(candidate.name, "\n");
    printf("Enter updated Political Party: ");
    fgets(candidate.party, sizeof(candidate.party), stdin);
    strtok(candidate.party, "\n");
    printf("Enter updated Constituency: ");
    fgets(candidate.constituency, sizeof(candidate.constituency), stdin);
    strtok(candidate.constituency, "\n");

    fseek(file, -strlen(candidate.name) - strlen(candidate.party) - strlen(candidate.constituency) - sizeof(candidate.votes), SEEK_CUR);

    fprintf(file, "%s %s %s %d\n", candidate.name, candidate.party, candidate.constituency, candidate.votes);

    fclose(file);

    printf("Candidate details updated successfully.\n");
}

void deleteCandidate()
{
    char name[50];
    printf("Enter the name of the candidate you want to delete: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    strtok(name, "\n");

    FILE *file = fopen("candidatelist.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    struct Candidate candidate;
    int found = 0;

    while (fscanf(file, "%s %s %s %d\n", candidate.name, candidate.party, candidate.constituency, &candidate.votes) != EOF)
    {
        if (strcmp(candidate.name, name) == 0)
        {
            found = 1;
            printf("Candidate %s deleted successfully.\n", name);
            continue;
        }
        fprintf(tempFile, "%s %s %s %d\n", candidate.name, candidate.party, candidate.constituency, candidate.votes);
    }

    fclose(file);
    fclose(tempFile);

    remove("candidatelist.txt");

    if (rename("temp.txt", "candidatelist.txt") != 0)
    {
        printf("Error updating candidate list. Please make sure no other programs are accessing the file.\n");
        return;
    }

    if (!found)
    {
        printf("Candidate with name %s not found.\n", name);
    }
}
void voterMenu()
{
    int choice;
    do
    {
        printf("\nVoter Menu\n");
        printf("1. Register Voter\n");
        printf("2. Update Voter Details\n");
        printf("3. Search Voter Details\n");
        printf("4. Cast Vote\n");
        printf("5. View Vote Result\n");
        printf("6. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            registerVoter();
            break;
        case 2:
            updateVoterDetails();
            break;
        case 3:
            searchVoterDetails();
            break;
        case 4:
            castVote();
            break;
        case 5:
            viewVoteResult();
            break;
        case 6:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while(choice != 6);
}


void registerVoter() {
    FILE *file = fopen("voterlist.txt", "a+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int numVoters = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        numVoters++;
    }

    if (numVoters >= 100) {
        printf("Maximum number of voters reached. Cannot register more voters.\n");
        fclose(file);
        return;
    }

    fclose(file);
    file = fopen("voterlist.txt", "a+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Voter newVoter;

    printf("Enter Voter Name: ");
    getchar();
    fgets(newVoter.name, sizeof(newVoter.name), stdin);
    strtok(newVoter.name, "\n");

    printf("Enter Date of Birth (YYYY MM DD): ");
    scanf("%d %d %d", &newVoter.dob.year, &newVoter.dob.month, &newVoter.dob.day);

    printf("Enter Address: ");
    getchar();
    fgets(newVoter.address, sizeof(newVoter.address), stdin);
    strtok(newVoter.address, "\n");

    printf("Enter Password: ");
    fgets(newVoter.password, sizeof(newVoter.password), stdin);
    strtok(newVoter.password, "\n");

    numVoters++;

    fprintf(file, "%d\t%s\t%d %d %d\t%s\t%s\n", numVoters, newVoter.name, newVoter.dob.year, newVoter.dob.month, newVoter.dob.day, newVoter.address, newVoter.password);

    fclose(file);

    printf("Voter registered successfully. Serial Number: %d\n", numVoters);

}



void updateVoterDetails()
{
    FILE *file = fopen("voterlist.txt", "r+");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int sno;
    printf("Enter the Serial Number of the voter you want to update: ");
    scanf("%d", &sno);

    struct Voter voter;
    int found = 0;
    long int position = ftell(file);

    while (fscanf(file, "%d\t%s\t%d %d %d\t%s\t%s\n", &voter.sno, voter.name, &voter.dob.year, &voter.dob.month, &voter.dob.day, voter.address, voter.password) != EOF)
    {
        if (voter.sno == sno)
        {
            found = 1;
            break;
        }
        position = ftell(file);
    }

    if (!found)
    {
        printf("Voter with Serial Number %d not found.\n", sno);
        fclose(file);
        return;
    }

    while ((getchar()) != '\n');

    printf("Enter new Voter Name: ");
    fgets(voter.name, sizeof(voter.name), stdin);
    strtok(voter.name, "\n");

    printf("Enter new Date of Birth (YYYY MM DD): ");
    scanf("%d %d %d", &voter.dob.year, &voter.dob.month, &voter.dob.day);

    while ((getchar()) != '\n');

    printf("Enter new Address: ");
    fgets(voter.address, sizeof(voter.address), stdin);
    strtok(voter.address, "\n");

    printf("Enter new Password: ");
    fgets(voter.password, sizeof(voter.password), stdin);
    strtok(voter.password, "\n");

    fseek(file, position, SEEK_SET);

    fprintf(file, "%d\t%s\t%d %d %d\t%s\t%s\n", voter.sno, voter.name, voter.dob.year, voter.dob.month, voter.dob.day, voter.address, voter.password);

    fclose(file);

    printf("Voter details updated successfully.\n");
}

void searchVoterDetails()
{
    FILE *file = fopen("voterlist.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int searchSno;
    printf("Enter the Serial Number of the voter you want to search: ");
    scanf("%d", &searchSno);

    struct Voter voter;
    int found = 0;

    while (fscanf(file, "%d\t%s\t%d %d %d\t%*s\t%*s\n", &voter.sno, voter.name, &voter.dob.year, &voter.dob.month, &voter.dob.day) != EOF)
    {
        if (voter.sno == searchSno)
        {
            found = 1;
            printf("Voter Details:\n");
            printf("Serial Number: %d\n", voter.sno);
            printf("Name: %s\n", voter.name);
            printf("Date of Birth: %d/%d/%d\n", voter.dob.year, voter.dob.month, voter.dob.day);
            break;
        }
    }

    if (!found)
    {
        printf("Voter with Serial Number %d not found.\n", searchSno);
    }

    fclose(file);
}

void castVote() {
    struct Voter voters[100];
    struct Candidate candidates[50];
    int numVoters = 0;
    int numCandidates = 0;

    FILE *voterFile = fopen("voterlist.txt", "r");
    if (voterFile == NULL) {
        printf("Error opening voterlist.txt file.\n");
        return;
    }

    while (fscanf(voterFile, "%d\t%s\t%d %d %d\t%s\t%s\n", &voters[numVoters].sno, voters[numVoters].name, &voters[numVoters].dob.year, &voters[numVoters].dob.month, &voters[numVoters].dob.day, voters[numVoters].address, voters[numVoters].password) != EOF) {
        numVoters++;
        if (numVoters >= 100) {
            break;
        }
    }

    fclose(voterFile);

    FILE *candidateFile = fopen("candidatelist.txt", "r");
    if (candidateFile == NULL) {
        printf("Error opening candidatelist.txt file.\n");
        return;
    }

    while (fscanf(candidateFile, "%s %s %s %d\n", candidates[numCandidates].name, candidates[numCandidates].party, candidates[numCandidates].constituency, &candidates[numCandidates].votes) != EOF) {
        numCandidates++;
        if (numCandidates >= 50) {
            break;
        }
    }

    fclose(candidateFile);

    int sno, choice;
    char password[20];
    int votedIndex = -1;

    printf("Enter your Serial Number: ");
    scanf("%d", &sno);
    printf("Enter your Password: ");
    scanf("%s", password);

    for (int i = 0; i < numVoters; i++) {
        if (voters[i].sno == sno && strcmp(voters[i].password, password) == 0) {
            votedIndex = i;
            break;
        }
    }

    if (votedIndex == -1) {
        printf("Invalid Serial Number or Password. Please try again.\n");
        return;
    }

    if (voters[votedIndex].voted == 1) {
        printf("You have already cast your vote.\n");
        return;
    }

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    int currentYear = localTime->tm_year + 1900;
    int voterYear = voters[votedIndex].dob.year;
    if (currentYear - voterYear < 18) {
        printf("You must be 18 years or older to cast your vote.\n");
        return;
    }

    printf("List of Candidates:\n");
    for (int i = 0; i < numCandidates; i++) {
        if (strcmp(candidates[i].constituency, voters[votedIndex].address) == 0) {
            printf("%d. %s\n", i + 1, candidates[i].name);
        }
    }

    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > numCandidates) {
        printf("Invalid choice. Please select a valid candidate.\n");
        return;
    }

    candidates[choice - 1].votes++;
    voters[votedIndex].voted = 1;

    FILE *voteCountFile = fopen("votecount.txt", "a");
    if (voteCountFile == NULL) {
        printf("Error opening votecount.txt file.\n");
        return;
    }

    fprintf(voteCountFile, "%s\t%d\n", candidates[choice - 1].name, candidates[choice - 1].votes);

    fclose(voteCountFile);

    printf("Your vote has been cast successfully.\n");
}

void viewVoteResult()
{
    printf("\nVote Results:\n");

    FILE *file = fopen("votecount.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);
}



