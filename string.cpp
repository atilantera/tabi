#ifndef STRING_CPP
#define STRING_CPP

int strcmp2(char * s1, char * s2)
{
	// Returns 1 if s1[i] is equal to s2[i]
	// in range of [0, strlen(s2)-1]

	// Returns 0 if false or strlen(s2) > strlen(s1)

	if(s1 != NULL && s2 != NULL)
	{
		int len1 = strlen(s1);
		int len2 = strlen(s2);

		if(len2 > len1) return 0;
		else
		{
			for(int i = 0; i < len2; i++)
				if(s1[i] != s2[i]) return 0;
		}
		return 1;
	}
	return 0;
}

void mergestr(char * s1, char * s2, char * merged)
{
	// Returns strings s1 and s2 merged in * merged.

	int len1;
	int len2;
	int i;

	if(merged != NULL) delete [] merged;
	merged = 0;

	if(s1 == NULL) len1 = 0;
	else len1 = strlen(s1);

	if(s2 == NULL) len2 = 0;
	else len2 = strlen(s2);

	if(len1 > 0 && len2 == 0)
	{
		merged = new char [len1 + 1];
		if(merged != NULL) strcpy(merged, s1);
	}

	if(len1 == 0 && len2 > 0)
	{
		merged = new char [len2 + 1];
		if(merged != NULL) strcpy(merged, s2);
	}

	if(len1 > 0 && len2 > 0)
	{
		merged = new char[len1 + len2 + 1];
		if(merged != NULL)
		{
			for(i = 0; i < len1; i++) merged[i] = s1[i];
			for(i = len1; i < len1+len2; i++) merged[i] = s2[i];
			merged[len1+len2] = '\0';
		}
	}
}

#endif
