#include<stdio.h>
#include<math.h>

#define MAX 100 // Maximun number of chekis
#define cheki_time 90 // 90 seconds
#define total_time 7200 // 2 hours (7200 seconds)

int selected_chekis[MAX] = { 0 }; // Number of selected chekis per idol

typedef struct {
	char name[20];
	int preference;
	int queue_length;
	int cheki_count;
} Idol;

Idol idols[MAX];

int cheki_count[MAX] = { 0 };

typedef struct {
	int index; // Index of the idol
	double priority; // Calculated priority score
} ScheduleEntry;

ScheduleEntry schedule[MAX];

void input_idol_data(int n) {
	for (int i = 0; i < n; i++) {
		printf("[Information for Idol #%d]\n", i+1);
		
		printf("Name: ");
		scanf_s("%s", idols[i].name, (unsigned)sizeof(idols[i].name));

		printf("Preference(0 or 1): ");
		scanf_s("%d", &idols[i].preference);

		printf("Queue length: ");
		scanf_s("%d", &idols[i].queue_length);

		printf("Number of chekis: ");
		scanf_s("%d", &idols[i].cheki_count);

		printf("\n");
	}
}

void knapsack_selection(int n) {
	int remaining_time = total_time;
	int i;

	// Step 1
	// Ensure at least one cheki is selected for idols with preference == 1
	for (i = 0; i < n; i++) {
		int queue_time = cheki_time * (idols[i].queue_length + 1);
		if (idols[i].preference == 1&&idols[i].cheki_count>0){
			if (remaining_time < queue_time) {
				printf("Not enough time to include %s", idols[i].name);
				return;
			}
			selected_chekis[i]++;
			remaining_time -= queue_time;
			idols[i].cheki_count--;
		}
	}

	// Step 2
	// Use remaining time to greedily selected chekis based on efficiency
	while (1) {
		int best_index = -1;
		double best_score = 0.0;

		for (int i = 0; i < n; i++) {
			int queue_time = cheki_time * (idols[i].queue_length + 1);
			// if additional chekis can be selected and time allows
			if (idols[i].cheki_count > 0 && remaining_time >= queue_time) {
				double score = 0.5 * (double)idols[i].preference + 0.1 * (double)idols[i].cheki_count;

				if (score > best_score) {
					best_score = score;
					best_index = i;
				}
			}
		}

		// No more valid selections possible
		if (best_index == -1) break;

		// Select one more cheki for the best idol
		selected_chekis[best_index]++;
		idols[best_index].cheki_count--;
		remaining_time -= cheki_time * (idols[best_index].queue_length + 1);
	}

	printf("\n=== Selected Chekis ===\n");
	for (int i = 0; i < n; i++) {
		printf("%s: %d chekis\n", idols[i].name, selected_chekis[i]);
	}
	printf("Total time: %d seconds\n", total_time-remaining_time);
	printf("Remaining time: %d seconds\n", remaining_time);
}

void priority_scheduling(int n) {
	int schedule_size = 0;

	// Step 1
	// Flatten selected chekis into schedule array with cost
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < selected_chekis[i]; j++) {
			schedule[schedule_size].index = i;
			schedule[schedule_size].priority = ((double)idols[i].preference + 1) * 50.0 + (double)cheki_count[i] * 1.5 - ((double)idols[i].queue_length / 5);
			schedule_size++;
			cheki_count[i]--;
		}
	}

	// Step 2
	// Sort
	for (int i = 0; i < schedule_size - 1; i++) {
		for (int j = 0; j < schedule_size - i - 1; j++) {
			if (schedule[j].priority < schedule[j + 1].priority) {
				ScheduleEntry temp = schedule[j];
				schedule[j] = schedule[j + 1];
				schedule[j + 1] = temp;
			}
		}
	}

	// Step 3
	// Print final schedule and compute average waiting time
	printf("\n=== Cheki Order ===\n");
	for (int i = 0; i < schedule_size; i++) {
		int idx = schedule[i].index;
		printf("%2d. %s (Priority: %.2f, Queue: %ds)\n", i + 1, idols[idx].name, schedule[i].priority, idols[idx].queue_length * cheki_time);
	}
}

void print_test(int n) {
	printf("\n=== Idol Information Summary ===\n");
	for (int i = 0; i < n; i++) {
		printf("Name: %s, Preference : %d, Queue length : %d, Cheki number : %d\n",
			idols[i].name,
			idols[i].preference,
			idols[i].queue_length,
			idols[i].cheki_count);
	}
}

int main(){
	int n; // The number of idols

	printf("Enter the number of idols: ");
	scanf_s("%d", &n);

	if (n > MAX) {
		printf("The number of idols must not exceed %d.", MAX);
		return 1;
	}

	for (int i = 0; i < n; i++) {
		cheki_count[i] = idols[i].cheki_count;
	}

	input_idol_data(n);

	print_test(n);

	knapsack_selection(n);

	priority_scheduling(n);

	return 0;
}