#include <iostream>
#include <cctype>
#include <fstream>
using namespace std;

void getWordOriginal(char line[], int& i, char word[]) {
	int j = 0;
	char set[9] = ".,: !?-";
	char *p = strpbrk(line, set);
	if (p == NULL) {
		while (line[i] != '\0') {
			word[j] = line[i];
			i++;;
			j++;
		}
		return;
	}
	do {
		word[j] = line[i];
		j++;
	} while (line[i++] != *p);
	if (line[i] == '.' || line[i] == ',' || line[i] == ':' || line[i] == '!' || line[i] == '?' || line[i] == '-') {
		word[j++] = line[i++];
	}
	word[j] = '\0';

	for (int k = 0; k <= strlen(word); k++) {
		cerr << word[k];
	}
	cerr << "d" << endl;
}

int getWordPortion(const char line[], int& i, char word[]) {
	while (i < strlen(line) && isspace(line[i])) {
		i++;
	}
	int temp = 0;
	int j = 0;
	while (i < strlen(line) && !isspace(line[i])) {
		word[j++] = line[i++];
		if (word[j - 1] == '-') {
			if (i < strlen(line) && !isspace(line[i])) {	
				temp = 1;
			}
			break;
		}
	}
	word[j] = '\0';
	return temp;
}

bool skipParagraphs(char line[], int& i, istream& inf) {
	int j;
	char word[200];
	while (true) {
		j = i;
		getWordPortion(line, i, word);
		if (word[0] == '\0') {
			if (!inf.getline(line, 200)) {
				return false;
			}
			i = 0;
			continue;
		}
		if (strcmp(word, "@P@") != 0) {
			i = j;
			break;
		}
	}
	return true;
}

int checkLine(int lineLength, char line[], int& i, istream& inf, ostream& outf, int& charCount) {
	char word[200];
	int prevCheck = 0;
	while (i < strlen(line)) {
		int check = getWordPortion(line, i, word);  
		if (strcmp(word, "@P@") == 0 && prevCheck == 0) {
			if (!skipParagraphs(line, i, inf)) {
				break;
			}
			outf << endl << endl;
			charCount = 0;
			continue;
		}
		cerr << word << " charcount " << charCount << " line length " << lineLength << endl;
		if (charCount + strlen(word) <= lineLength) {
			if (charCount != 0 && prevCheck == 0) {
				outf << " ";
				charCount++;
			}
			outf << word;
			charCount += strlen(word);
		}
		else {
			outf << endl << word;
			charCount = strlen(word);
		}
		prevCheck = check;
	}
	return 0;
}


int checkLineOriginal(int lineLength, char line[], ostream& outf, int& rollover) {
	int i = 0;
	int temp = 0;
	int paraCounter = 0;
	int wordCounter = 0;
	int spaceCounter = 0;
	char* last = NULL;
	while (line[i] != '\0') {
		char word[200];
		if (i >= strlen(line)) {
			break;
		}
		getWordPortion(line, i, word);
		if(strcmp(word, "@p@") == 0 && paraCounter == 0) {
			paraCounter++;
			outf << endl << endl;
			wordCounter = 0;
		}
		else if (strcmp(word, "@p@") != 0) {
			paraCounter = 0;
		}
		if (strcmp(word, " ") != 0) {
			if (wordCounter != 0 && spaceCounter == 0 && strcmp(word, "@p@") != 0 && strlen(word) <= lineLength + 1 - rollover) {
				if (last != NULL && (*last == '!' || *last == '.' || *last == '?' || *last == ':') && strlen(word) <= lineLength - rollover - 2) {
					outf << " ";
				}
				if (strlen(word) <= lineLength - rollover - 1 && last != NULL && (*last != '!' && *last != '.' && *last != '?' && *last != ':')) {
					outf << " ";
				}
				spaceCounter++;
			}
			if (strlen(word) > (lineLength + 1) && strcmp(word, "@p@") != 0 && strcmp(word, " ") != 0) {
				temp = 1;
				size_t k = 0;
				int j = 0;
				while (k < strlen(word)) {
					for (j = 0; j < lineLength; j++) {
						if (word[k] == '\0') {
							rollover = j + 1;
							if (k > 0) {
								last = &word[k - 1];
							}
							k++;
							break;
						}
						outf << word[k++];
					}
				}
				if (rollover >= lineLength - 1 || (last != NULL && (*last == '!' || *last == '.' || *last == '?' || *last == ':') && rollover >= lineLength - 2)) {
					rollover = 0;
					wordCounter = 0;
					last = NULL;
					outf << endl;
				}
				else {
					wordCounter++;
				}
				spaceCounter = 0;
			} 
			else if (strlen(word) <= lineLength + 1 - rollover && strcmp(word, "@p@") != 0 && strcmp(word, " ") != 0) {
				int p = 0;
				while (word[p] != '\0') {
					outf << word[p++];
				}
				if (p > 0) {
					last = &word[p - 1];
				}
				rollover += strlen(word);
				if (last != NULL && (rollover >= lineLength - 1 || ((*last == '!' || *last == '.' || *last == '?' || *last == ':') && rollover >= lineLength - 2))) {
					outf << endl;
					rollover = 0;
					wordCounter = 0;
					last = NULL;
				}
				else {
					wordCounter++;
				}
				spaceCounter = 0;
			}
			else if (strlen(word) > lineLength + 1 - rollover && strcmp(word, "@p@") != 0 && strcmp(word, " ") != 0) {
				outf << endl;
			}
		}
	}
	if (temp == 1) {
		return 1;
	}
	return 0;
}

int render(int lineLength, istream& inf, ostream& outf) {
	if (lineLength < 1) {
		return 2;
	}
	const int MAX = 200;
	char line[MAX];
	int temp = 0;
	int result = 0;
	int charCount = 0;
	while (inf.getline(line, MAX))
	{
		int i = 0;
		temp = checkLine(lineLength, line, i, inf, outf, charCount);
		if (temp == 1) {
			result = 1;
		}
	}
	return result;
}

int main() {
	ifstream infile("test.txt");
	ofstream outfile("result.txt");
	int lineLength;
	cin >> lineLength;
	cout << render(lineLength, infile, outfile);
	return 0;
}