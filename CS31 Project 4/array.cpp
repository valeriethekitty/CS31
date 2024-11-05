#include <iostream>
#include <string> 
#include <cassert>
using namespace std;

int reduplicate(string a[], int n) {
	if (n < 0) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		a[i] += a[i];
	}
	return n;
}

int locate(const string a[], int n, string target) {
	for (int i = 0; i < n; i++) {
		if (a[i] == target) {
			return i;
		}
	}
	return -1;
}

bool isGreatest(const string a[], int n, string s) {
	for (int i = 0; i < n; i++) {
		if (s < a[i]) {
			return false;
		}
	}
	return true;
}

int locationOfMax(const string a[], int n) {
	for (int i = 0; i < n; i++) {
		if (isGreatest(a, n, a[i])) {
			return i;
		}
	}
	return -1;
}

int circleLeft(string a[], int n, int pos) {
	if (n <= 0 || pos < 0 || pos >= n) {
		return -1;
	}
	string temp = a[pos];
	for (int i = pos; i < n - 1; i++) {
		a[i] = a[i + 1];
	}
	a[n - 1] = temp;
	return pos;
}

int enumerateRuns(const string a[], int n) {
	if (n < 0) {
		return -1;
	}
	if (n == 0) {
		return 0;
	}
	string head = a[0];
	int runs = 1;
	for (int i = 0; i < n; i++) {
		if (a[i] != head) {
			runs++;
			head = a[i];
		}
	}
	return runs;
}

int flip(string a[], int n) {
	if (n < 0) {
		return -1;
	}
	for (int i = 0; i < n / 2; i++) {
		string temp = a[i];
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
	return n;
}

int locateDifference(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) {
		return -1;
	}
	int n;
	if (n2 > n1) {
		n = n1;
	}
	else {
		n = n2;
	}
	for (int i = 0; i < n; i++) {
		if (a1[i] != a2[i]) {
			return i;
		}
	}
	return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0 || n2 > n1) {
		return -1;
	}
	if (n2 == 0) {
		return 0;
	}
	int i = 0;
	while (i <= n1 - n2) {
		int k = i;
		for (int j = 0; j < n2; j++) {
			if (a1[k] == a2[j]) {
				k++;
			}
			else if (a1[k] != a2[j]) {
				i++;
				break;
			}
			if (k == i + n2) {
				return i;
			}
		}
	}
	return -1;
}

int locateAny(const string a1[], int n1, const string a2[], int n2) {
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < n2; j++) {
			if (a1[i] == a2[j]) {
				return i;
			}
		}
	}
	return -1;
}

int divide(string a[], int n, string divider) { /* fix this */
	if (n <= 0) {
		return -1;
	}
	int i = 0;
	int j = n - 1;
	while (i < j) {
		if (a[i] >= divider) {
			string temp = a[j];
			a[j] = a[i];
			a[i] = temp;
			j--;
		}
		else {
			i++;
		}
	}
	for (i = 0; i < n; i++) {
		if (a[i] > divider) {
			return i;
		}
	}
	return n;
}

int main() {
	string h[7] = { "nikki", "ron", "asa", "vivek", "", "chris", "donald" };
	assert(locate(h, 7, "chris") == 5);
	assert(locate(h, 7, "asa") == 2);
	assert(locate(h, 2, "asa") == -1);
	

	string g[4] = { "nikki", "ron", "chris", "tim" };
	assert(locateDifference(h, 4, g, 4) == 2);
	
	assert(circleLeft(g, 4, 1) == 1 && g[1] == "chris" && g[3] == "ron");

	string c[4] = { "ma", "can", "tu", "do" };
	assert(reduplicate(c, 4) == 4 && c[0] == "mama" && c[3] == "dodo");

	string e[4] = { "asa", "vivek", "", "chris" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "ron", "ron", "ron", "chris", "chris" };
	assert(enumerateRuns(d, 5) == 2);

	string f[3] = { "vivek", "asa", "tim" };
	assert(locateAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "tim" && f[2] == "vivek");

	assert(divide(h, 7, "donald") == 3);
	
	assert(locationOfMax(h, 7) == 3);

	cout << "All tests succeeded" << endl;
	return 0;
}