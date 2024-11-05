#include <iostream>
#include <cctype>
#include <fstream>
using namespace std;

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

int checkLine(int lineLength, char line[], int& i, istream& inf, ostream& outf, int& charCount, bool& first, char& prevChar) {
	char word[200];
	int prevCheck = 0;
	char temp = 0;
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
		int spaces = 0;
		if (charCount != 0 && prevCheck == 0) {
			spaces++;
			if (strchr(".:?!", prevChar) != NULL) {
				spaces++;
			}
		}
		cerr << word << " charcount " << charCount << " line length " << lineLength << endl;
		if (charCount + strlen(word) + spaces <= lineLength) {
			for (int k = 0; k < spaces; k++) {
				outf << " ";
			}
			outf << word;
			charCount = charCount + strlen(word) + spaces;
		}
		else {
			if (first == false) {
				outf << endl;
			}
			if (strlen(word) <= lineLength) {
				outf << word;
				charCount = strlen(word);
			}
			else {
				temp = 1;
				int p = 0;
				while (p < strlen(word)) {
					int q = 0;
					while (q < lineLength) {
						outf << word[p++];
						q++;
						if (word[p] == '\0') {
							break;
						}
					}
					if (word[p] == '\0') {
						charCount = q;
						break;
					}
					else {
						outf << endl;
					}
				}
			}
		}
		prevCheck = check;
		prevChar = word[strlen(word) - 1];
		first = false;
	}
	return temp;
}

int render(int lineLength, istream& inf, ostream& outf) {
	if (lineLength < 1) {
		return 2;
	}
	const int MAX = 200;
	char line[MAX];
	int temp = 0;
	bool outputNothing = false;
	int result = 0;
	int charCount = 0;
	bool first = true;
	bool firstWord = true;
	char prevChar = 'k';
	while (inf.getline(line, MAX))
	{
		int i = 0;
		if (first) {
			if (!skipParagraphs(line, i, inf)) {
				outputNothing = true;
				break;
			}	
			first = false;
		}
		temp = checkLine(lineLength, line, i, inf, outf, charCount, firstWord, prevChar);
		if (temp == 1) {
			result = 1;
		}
	}
	if (outputNothing == false) {
		outf << endl;
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