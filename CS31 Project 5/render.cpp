#include <iostream>
#include <cctype>
#include <fstream>
using namespace std;

int getWordPortion(const char line[], int& i, char word[]) { 
	while (i < strlen(line) && isspace(line[i])) { /* skip spaces at the beginning of a word portion */
		i++; /* increment position */
	}
	int temp = 0; 
	int j = 0;
	while (i < strlen(line) && !isspace(line[i])) { /* loop until a space or the end of the line */
		word[j++] = line[i++]; /* set next character in word to the character in line and then increment i and j */
		if (word[j - 1] == '-') { 
			if (i < strlen(line) && !isspace(line[i])) { /* if the hyphen is not at the end of the word */
				temp = 1; /* set return value to 1 */
			}
			break; 
		}
	}
	word[j] = '\0'; /* end word portion with \0 */
	return temp; /* return 0 if not end in hyphen and 1 if end in hyphen */
}

bool skipParagraphs(char line[], int& i, istream& inf) {
	int j;
	char word[200];
	while (true) {
		j = i; /* set j to current position */
		getWordPortion(line, i, word); /* get the next word portion */
		if (word[0] == '\0') { /* if you get to end of the line */
			if (!inf.getline(line, 200)) { /* get next line, and if EOF, return false */
				return false;
			}
			i = 0; /* set current position in the line to 0 */
			continue;
		}
		if (strcmp(word, "@P@") != 0) { /* if it is not  @P@, set i to previously saved position */
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
	while (i < strlen(line)) { /* loop through the line */
		int check = getWordPortion(line, i, word); /* get the next word */
		if (word[0] == '\0') {
			break;
		}
		if (strcmp(word, "@P@") == 0 && prevCheck == 0) { /* if the word is @P@ and previous check is 0*/
			if (!skipParagraphs(line, i, inf)) { /* call skip line function, if false, get out of the loop */
				break;
			}
			outf << endl << endl; /* otherwise output two new lines */
			charCount = 0; /* reset character count */
			continue;
		}
		int spaces = 0;
		if (charCount != 0 && prevCheck == 0) { /* increment spaces based on previous word's last character */
			spaces++;
			if (strchr(".:?!", prevChar) != NULL) {
				spaces++;
			}
		}
		if (charCount + strlen(word) + spaces <= lineLength) { /* check if word fits on the same line */
			for (int k = 0; k < spaces; k++) { /* add appropriate spaces */
				outf << " ";
			}
			outf << word; /* add the word */
			charCount = charCount + strlen(word) + spaces; /* adjust character count */
		}
		else {
			if (first == false) { /* if not first line, output new line */
				outf << endl;
			}
			if (strlen(word) <= lineLength) { /* if the length of the word is less than the lineLength, output the word */
				outf << word; 
				charCount = strlen(word); /* adjust character count */
			}
			else {
				temp = 1;
				int p = 0;
				while (p < strlen(word)) { /* loop through the word */
					int q = 0; /* set q to 0 */
					while (q < lineLength) { /* loop through the line and output each character in word until the line runs out of space */
						outf << word[p++];
						q++;
						if (word[p] == '\0') { /* if it is at the end of the word, break */
							break;
						}
					}
					if (word[p] == '\0') { /* if it is at the end of the word, set charCount and break */
						charCount = q;
						break;
					}
					else {
						outf << endl; /* otherwise output new line */
					}
				}
			}
		}
		prevCheck = check; /* set prevCheck, prevChar */
		prevChar = word[strlen(word) - 1];
		first = false; /* first is now false */
	}
	return temp; 
}

int render(int lineLength, istream& inf, ostream& outf) {
	if (lineLength < 1) { /* return 2 without doing anything if length is less than 1 */
		return 2;
	}
	const int MAX = 200; 
	char line[MAX]; /* initialize line to have 200 spaces, since line has a maximum of 180 characters, doing 200 to be safe */
	int temp = 0; /* set temp to 0 to represent the case where all words are below lineLength */
	int result = 0; /* set result to 0 */
	int charCount = 0; /* initialize charCount to 0 */
	bool first = true; /* first is true for the first line */
	bool firstWord = true; /* firstWord is true for the first word */
	char prevChar = 'k'; /* set prevChar to any random character not !.:?\0 */
	while (inf.getline(line, MAX)) /* while not at the EOF, get the line from input */
	{
		int i = 0; /* set position in line to 0 */
		if (first) { /* if its the first line */
			if (!skipParagraphs(line, i, inf)) { /* call skip paragraphs, if there is nothing else but @P@ then set outputNothing to true and break out of loop */
				break;
			}	
			first = false; /* otherwise set first to false */
		}
		temp = checkLine(lineLength, line, i, inf, outf, charCount, firstWord, prevChar); /* set temp to checkLine */
		if (temp == 1) { /* if temp is 1, there is a word greater than lineLength, so result = 1 */
			result = 1;
		}
	}
	if (!first) { /* if first is true, you should not output a newline, if it is false you should */
		outf << endl;
	}
	return result; /* returns either 0 or 1 depending on word length */
}

int main() {
	ifstream infile("test.txt");
	ofstream outfile("result.txt");
	int lineLength;
	cin >> lineLength;
	cout << render(lineLength, infile, outfile);
	return 0;
}