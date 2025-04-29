#ifndef TAB_CPP
#define TAB_CPP

#include <conio.h>	// clrscr()
#include <stdlib.h> // for _ultoa()
#include "debug.hpp"

TabBar::TabBar()
{
	length = 0;
	deltaStart = 0;
	deltaLength = 0;

	numerator = 0;
	denominator = 0;
	tempo = 0;

	NoError = 1;
}

/*
TabBar::TabBar(const TabBar &op)
{
	length = op.length;
	deltaStart = op.deltaStart;
	deltaLength = op.deltaLength;

	numerator = op.numerator;
	denominator = op.denominator;
	tempo = op.tempo;

	NoError = op.NoError;

	if (NoError)
	{
		Notes.reserve(op.Notes.size());
		Notes = op.Notes;
	}
}

TabBar & TabBar::operator= (const TabBar &op)
{
	length = op.length;
	deltaStart = op.deltaStart;
	deltaLength = op.deltaLength;

	numerator = op.numerator;
	denominator = op.denominator;
	tempo = op.tempo;

	NoError = op.NoError;

	if (NoError) {
		Notes.reserve(op.Notes.size());
		// Notes.operator=(op.Notes);
		Notes = op.Notes;
		cout << "tab/50: this_size=" << Notes.size() << " op_size=" << op.Notes.size() << "\n";
		assert(Notes.size() == op.Notes.size());
	}

	return *this;
}
*/

void TabBar::addNotes(vector<TabNote> & allnotes)
{
	Notes.reserve(allnotes.size());
	Notes = allnotes;
	assert(Notes.size() == allnotes.size());
}

class TabNote TabBar::getNote(unsigned short num)
{
	if (num<Notes.size()) return Notes[num];
}

void TabBar::getUsed(char * used)
{
	// Returns key values of notes that exists in this bar
	// used[128] = char-boolean table for different key values
	// Marks used[x] = 1, if note which key value is x can be found in this bar
	// NOTE: this function only changes array used[]'s values to 1.
	//		 It doesn't format the array.

	if (!Notes.empty()) {

		unsigned short i;		// Counter
		short different = 0;	// Different key values found and marked
		TabNote note;

		i = 0;
		do {
			note = Notes[i];
			if (used[note.key] == 0)
			{
				used[note.key] = 1;
				different++;
			}
			i++;
		} while(i < Notes.size() && different < 128);
	}
}

TabOptions::TabOptions()
{
	minQsize = 4;
	tripletMode = 1;
	meterLine = 1;
	meterLimit = 16;
	meterChanges = 1;
	pageCols = 78;
	pageRows = 50;
	rowLimit = 1;
	pageNumbers = 1;
	writeDrumLegend = 1;
	drumNameLength = 2;
	rowsBetweenBars = 1;

	drumKit = new TabDrumkit;

	autoSelectTrack = 0;
	windowAutoClose = 0;
	inPath = 0;
	outPath = 0;
	titleAlign = 1;
	rowsBeforeTitle = 3;
	rowsForTitle = 7;
}

TabOptions::TabOptions(const TabOptions &op)
{
	minQsize = op.minQsize;
	tripletMode = op.tripletMode;
	meterLine = op.meterLine;
	meterLimit = op.meterLimit;
	meterChanges = op.meterChanges;
	pageCols = op.pageCols;
	pageRows = op.pageRows;
	rowLimit = op.rowLimit;
	pageNumbers = op.pageNumbers;
	writeDrumLegend = op.writeDrumLegend;
	drumNameLength = op.drumNameLength;
	rowsBetweenBars = op.rowsBetweenBars;

	drumKit = new TabDrumkit;
	if (drumKit!=NULL)
	{
		*drumKit = *op.drumKit;
	}

	autoSelectTrack = op.autoSelectTrack;
	windowAutoClose = op.windowAutoClose;
	setinpath(op.inPath);
	setoutpath(op.outPath);
	titleAlign = op.titleAlign;
	rowsBeforeTitle = op.rowsBeforeTitle;
	rowsForTitle = op.rowsForTitle;
}

TabOptions & TabOptions::operator= (const TabOptions &op)
{
	minQsize = op.minQsize;
	tripletMode = op.tripletMode;
	meterLine = op.meterLine;
	meterLimit = op.meterLimit;
	meterChanges = op.meterChanges;
	pageCols = op.pageCols;
	pageRows = op.pageRows;
	rowLimit = op.rowLimit;
	pageNumbers = op.pageNumbers;
	writeDrumLegend = op.writeDrumLegend;
	drumNameLength = op.drumNameLength;
	rowsBetweenBars = op.rowsBetweenBars;

	drumKit = new TabDrumkit;
	if (drumKit!=NULL)
	{
		*drumKit = *op.drumKit;
	}

	autoSelectTrack = op.autoSelectTrack;
	windowAutoClose = op.windowAutoClose;
	setinpath(op.inPath);
	setoutpath(op.outPath);
	titleAlign = op.titleAlign;
	rowsBeforeTitle = op.rowsBeforeTitle;
	rowsForTitle = op.rowsForTitle;

	return *this;
}

TabOptions::~TabOptions()
{
	delete drumKit;
	delete [] inPath;
	delete [] outPath;
}

void TabOptions::read(ifstream & infile)
{
	unsigned long l;
	double versionID;
	infile.read(&versionID, sizeof(versionID));

	if (versionID == VERSION_ID)
	{
		infile.read(&minQsize, sizeof(minQsize));
		infile.read(&tripletMode, sizeof(tripletMode));
		infile.read(&meterLine, sizeof(meterLine));
		infile.read(&meterLimit, sizeof(meterLimit));
		infile.read(&meterChanges, sizeof(meterChanges));
		infile.read(&pageCols, sizeof(pageCols));
		infile.read(&pageRows, sizeof(pageRows));
		infile.read(&rowLimit, sizeof(rowLimit));
		infile.read(&pageNumbers, sizeof(pageNumbers));
		infile.read(&writeDrumLegend, sizeof(writeDrumLegend));
		infile.read(&drumNameLength, sizeof(drumNameLength));
		infile.read(&rowsBetweenBars, sizeof(rowsBetweenBars));

		// drumKit->read(infile);

		infile.read(&autoSelectTrack, sizeof(autoSelectTrack));
		infile.read(&windowAutoClose, sizeof(windowAutoClose));

		// inPath
		infile.read(&l, sizeof(l));
		delete [] inPath;
		inPath = new char [l+1];
		if(inPath != NULL && l > 0) {
			infile.read(&inPath, l);
			inPath[l] = '\0';
		}

		// outPath
		infile.read(&l, sizeof(l));
		delete [] outPath;
		outPath = new char [l+1];
		if(outPath != NULL && l > 0) {
			infile.read(&outPath, l);
			outPath[l] = '\0';
		}

		infile.read(&titleAlign, sizeof(titleAlign));
		infile.read(&rowsBeforeTitle, sizeof(rowsBeforeTitle));
		infile.read(&rowsForTitle, sizeof(rowsForTitle));
	}
}

void TabOptions::write(ofstream & outfile)
{
	unsigned long l;
	double versionID = VERSION_ID;
	outfile.write(&versionID, sizeof(versionID));

	outfile.write(&minQsize, sizeof(minQsize));
	outfile.write(&tripletMode, sizeof(tripletMode));
	outfile.write(&meterLine, sizeof(meterLine));
	outfile.write(&meterLimit, sizeof(meterLimit));
	outfile.write(&meterChanges, sizeof(meterChanges));
	outfile.write(&pageCols, sizeof(pageCols));
	outfile.write(&pageRows, sizeof(pageRows));
	outfile.write(&rowLimit, sizeof(rowLimit));
	outfile.write(&pageNumbers, sizeof(pageNumbers));
	outfile.write(&writeDrumLegend, sizeof(writeDrumLegend));
	outfile.write(&drumNameLength, sizeof(drumNameLength));
	outfile.write(&rowsBetweenBars, sizeof(rowsBetweenBars));

	// drumKit->write(outfile);

	outfile.write(&autoSelectTrack, sizeof(autoSelectTrack));
	outfile.write(&windowAutoClose, sizeof(windowAutoClose));

	// inPath
	if(inPath != NULL) {
		l = strlen(inPath);
		outfile.write(&l, sizeof(l));
		if(l > 0) outfile.write(inPath, l);
	}
	else {
		l = 0;
		outfile.write(&l, sizeof(l));
	}

	// outPath
	if(outPath != NULL) {
		l = strlen(outPath);
		outfile.write(&l, sizeof(l));
		if(l > 0) outfile.write(outPath, l);
	}
	else {
		l = 0;
		outfile.write(&l, sizeof(l));
	}

	outfile.write(&titleAlign, sizeof(titleAlign));
	outfile.write(&rowsBeforeTitle, sizeof(rowsBeforeTitle));
	outfile.write(&rowsForTitle, sizeof(rowsForTitle));
}

void TabOptions::minQ(unsigned char minQ)
{
	if (minQ>=2 && minQ%2==0 && !(minQ%3==0))
		minQsize = minQ;
}

void TabOptions::tripletlength(unsigned char tl)
{
	if (tl == 1 && tl == 2) tripletMode = tl;
}

void TabOptions::meterline(unsigned char ml)
{
	if (ml <= 2) meterLine = ml;
}

void TabOptions::meterlimit(unsigned char ml)
{
	if (ml > 1) meterLimit = ml;
}

void TabOptions::meterchanges(unsigned char mc)
{
	if (mc <= 1) meterChanges = mc;
}

void TabOptions::cols(unsigned short Cols)
{
	if (Cols >= MINCOLS) pageCols = Cols;
}

void TabOptions::rows(unsigned short Rows)
{
	if (Rows >= MINROWS) pageRows = Rows;
}

void TabOptions::userows(unsigned char yesno)
{
	if (yesno==0 || yesno==1) rowLimit = yesno;
}

void TabOptions::pagenum(unsigned char yesno)
{
	if (yesno==0 || yesno==1) pageNumbers = yesno;
}

void TabOptions::writelegend(unsigned char yesno)
{
	if (yesno==0 || yesno==1) writeDrumLegend = yesno;
}

void TabOptions::drumnamelen(unsigned char len)
{
	drumNameLength = len;
}

void TabOptions::rowsbetwbars(unsigned char r)
{
	if (r > (unsigned char)pageCols / 2)
		rowsBetweenBars = (unsigned char) pageCols / 2;
	else
		rowsBetweenBars = r;
}

void TabOptions::drumkit(class TabDrumkit * Drumkit)
{
	if (drumKit!=NULL) *drumKit = *Drumkit;
}

void TabOptions::autoselect(unsigned char at)
{
	if (at==0 || at==1) autoSelectTrack = at;
}

void TabOptions::windowautoclose(unsigned char w)
{
	if (w < 2) windowAutoClose = w;
}

void TabOptions::setinpath(char * path)
{
	delete [] inPath;
	inPath = 0;
	if(path != NULL)
	{
		inPath = new char [strlen(path) + 1];
		if(inPath != NULL) strcpy(inPath, path);
	}
	cout << "new inPath=" << inPath << endl;
}

void TabOptions::setoutpath(char * path)
{
	delete [] outPath;
	outPath = 0;
	if(path != NULL)
	{
		outPath = new char [strlen(path) + 1];
		if(outPath != NULL) strcpy(outPath, path);
	}
	cout << "new outPath=" << outPath << endl;
}

void TabOptions::titlealign(unsigned char ta)
{
	if(ta <= 2) titleAlign = ta;
}

void TabOptions::rowsbeforetitle(unsigned char rbt)
{
	if(rbt < rowsForTitle) rbt = rowsBeforeTitle;
}

void TabOptions::rowsfortitle(unsigned char rft)
{
	rowsForTitle = rft;
	if(rowsBeforeTitle > rowsbeforetitle())
		rowsBeforeTitle = int(rowsForTitle / 2);
}

unsigned char TabOptions::rowsbeforetitle()
{
	if(rowsBeforeTitle > rowsbeforetitle())
		rowsBeforeTitle = int(rowsForTitle / 2);
	return rowsBeforeTitle;
}

unsigned char TabOptions::rowsfortitle()
{
	if(rowsForTitle > rows()) rowsForTitle = rows();
	return rowsForTitle;
}

void TabOptions::getinpath(char * path)
{
	if(path != NULL) delete [] path;
	if(inPath == NULL) path = 0;
	else {
		path = new char [strlen(inPath) + 1];
		if(path != NULL) strcpy(path, inPath);
	}
}

void TabOptions::getoutpath(char * path)
{
	if(path != NULL) delete [] path;
	if(outPath == NULL) path = 0;
	else {
		path = new char [strlen(outPath) + 1];
		if(path != NULL) strcpy(path, outPath);
	}
}

TabTrack::TabTrack()
{
	BarCount = 0;

	trackno = -1;
	condtrack = -1;

	NoError = 1;
	ErrorNum = 0;
	ErrorRef = 0;
}

TabTrack::TabTrack(const TabTrack &op)
{
	unsigned long i;

	NoError = op.NoError;
	ErrorNum = 0;
	ErrorRef = 0;

	BarCount = 0;

	if (NoError)
	{
		Bars.reserve(op.BarCount);
		if (Bars.empty())
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			BarCount = op.BarCount;
			copy(op.Bars.begin(), op.Bars.end(), Bars.begin());
		}
	}

	Conductor = op.Conductor;
	Notes = op.Notes;
	Track = op.Track;

	trackno = op.trackno;
	condtrack = op.condtrack;
}

TabTrack & TabTrack::operator= (const TabTrack &op)
{
	unsigned long i;

	NoError = op.NoError;
	ErrorNum = op.ErrorNum;
	ErrorRef = op.ErrorRef;

	BarCount = 0;
	if (NoError)
	{
		Bars.reserve(op.BarCount);
		if (Bars.empty())
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			BarCount = op.BarCount;
			copy(op.Bars.begin(), op.Bars.end(), Bars.begin());
		}
	}

	Conductor = op.Conductor;
	Notes = op.Notes;
	Track = op.Track;

	return *this;
}

void TabTrack::printerror() const
{
	cout << "Error " << ErrorNum << ": ";
	switch(ErrorNum)
	{
	case 1:
		cout << "Out of memory";
		break;

	// import()
	case 10:
		cout << "Track has no notes";
		break;

	// writetab()
	case 201:
		cout << "Can't create file to write";
		break;
	case 202:
		cout << "Length of bar " << ErrorRef <<
				" is greater than page width";
		break;
	}
	cout << endl;
}

void TabTrack::import(const MIDIFile * src, short track, TabOptions options)
{
	// Imports a MIDIConductorTrack containing tick-type
	// meter and tempo changes and converts it to single bars
	// so that every of them contains own data about meter and tempo

	// NOTE: SongLengthTicks is the time-length of the whole MIDI song
	// and its tick value can be greater than of the last meter/tempo conductor
	// in &ConductorTrack.

	Bars.clear();
	Bars2.clear();

	midiheader = src->getHeader();
	if (NoError && track < src->getTrackCount())
	{
		trackno = track;

		if (midiheader.getFormat()==2) condtrack = track;
		else condtrack = 0;

		Conductor = src->getCondTrk(condtrack);
		Notes = src->getNoteTrk(track);
		Track = src->getTrack(track);

		unsigned long tempoCount = 0;
		unsigned long meterCount = 0;
		unsigned long i = 0;			// delta counter

		unsigned long barLength = 0;	// in delta
		unsigned long songLength = 0;	// in delta
		unsigned long songNotes = 0;

		unsigned long retTime = 0;
		unsigned char retNum = 0;
		unsigned char retDen = 0;
		float retTempo = 0;

		// Checking time signature (and tempo)
		// Defaults are four quarters per bar, 120 quarters per min
		TabBar current(4, 4, 120);

		if (Conductor.getMeterCount()==0)

		// Checking song length using the last note
		songNotes = Notes.getNoteCount();

		unsigned long q;
		unsigned long r;

		for (r = 0; r < Notes.getNoteCount(); r++)
		{
			Notes.getNote(r, q, retNum);
		}

		songNotes = Notes.getNoteCount();

		if (songNotes == 0)
		{
			NoError = 0;
			ErrorNum = 10;
		}
		else
		{
			Notes.getNote(songNotes-1, songLength, retNum);
		}

		//
		// Convert and add bars from *src to Bars
		//

		i = 0;	// delta time counter

		do {
			// NOTE: times of changes of meter and tempo *must* be even with
			// .getDiv in MIDIConductorTrack (&cTrack)

			// Import time signature
			if (meterCount < Conductor.getMeterCount())
			{
				Conductor.getMeter(meterCount, retTime, retNum, retDen);
				if (retTime == i)
				{
					current.setNum(retNum);
					current.setDen(retDen);
					meterCount++;
				}
			}

			// Import tempo
			if (tempoCount < Conductor.getTempoCount())
			{
				Conductor.getTempo(tempoCount, retTime, retTempo);
				if (retTime == i)
				{
					current.setTempo(retTempo);
					tempoCount++;
				}
			}

			// Import delta timing
			barLength = long(current.getNum() * (4 / double(current.getDen())) * midiheader.getDiv());
			current.setTime(i);
			current.setTimeLen(barLength);

			// Add current bar to Bars (vector<TabBar>)
			if (NoError)
			{
				Bars.push_back(current);
				BarCount++;
				i += barLength;
			}

		} while (i < songLength + 1 && NoError == 1);
		songLength = i;

		// Import and quantize notes to bars
		unsigned long firstnote = 0;
		unsigned long notecount = 0;
		i = 0;

		char divErrors;
		char * errorBars;
		errorBars = new char[BarCount];

		if (errorBars == NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			divErrors = 0;
			while(i < BarCount && NoError)
			{
				notecount = barNoteCount(i, firstnote);
				if (notecount > 0)
				{
					errorBars[i] = quantizeBar(i, firstnote, notecount, options);
					if (errorBars[i] == 1) divErrors = 1;
				}
				else {
					// If bar has no notes, just add it to Bars2[]
					TabBar thisBar;
					thisBar = Bars[i];
					thisBar.setCharPDen((4 / Bars[i].getDen()) * options.minQ());
					Bars2.push_back(thisBar);
				}
				i++;
				firstnote += notecount;
			}

			// List bars that had problems with quantization
			if (divErrors == 1)
			{
				char isRange = 0;

				cout << "Following bars were problematic " <<
						"when quantizing notes (bars=" << BarCount << "):" << endl;

				for (i = 0; i < BarCount; i++)
				{
					if (errorBars[i] == 1)
					{
						if (isRange == 1) isRange = 2;
						if (isRange == 0)
						{
							cout << i+1;
							isRange = 1;
						}
						if (isRange == 2 && i == BarCount-1)
						{
							cout << "-" << i+1;
						}
					}
					else
					{
						if (isRange == 2)
						{
							cout << "-" << i;
						}
						if (isRange > 0)
						{
							cout << " ";
							isRange = 0;
						}
					}
				}
				cout << endl;
			}
		}

	}
}

unsigned long TabTrack::barNoteCount(unsigned long barno, unsigned long firstnote)
{
	// Search in Bars and in Notes
	// Returns number of notes should be in this bar
	// barno	 = index number of this bar
	// firstnote = index of first possible note in this bar

	if (barno >= Bars.size() || Notes.getNoteCount()==0 || NoError == 0) return 0;

	unsigned long lastTick; 	  // The end of this bar's time range
	unsigned long notecount;	  // The value to be returned
	unsigned char key;			  // Because of MIDINoteTrack::getNote()
	unsigned long time;

	lastTick = Bars[barno].getTime() + Bars[barno].getTimeLen() - 1;

	notecount = 0;
	do {
		Notes.getNote(firstnote + notecount, time, key);

		if (time <= lastTick) notecount++;
		else break;
	} while(firstnote + notecount < Notes.getNoteCount());

	return notecount;
}

char TabTrack::quantizeBar(unsigned long barno, unsigned long firstnote,
	 unsigned long notecount, const TabOptions & options)
{
	// Does the final conversion from MIDI events to tabulature data
	// barno	   = index of this bar: Bars[barno]
	// notecount   = note count for this bar
	// firstnote   = indexes of first and last note of this bar in Bars[barno]
	// options	   = tabbing options, some limits for specific values

	// Returns 1 if there was problems with the quantization,
	// otherwise return 0.

	unsigned long firstTick;	  // The beginning of the time range of this bar
	unsigned long lastTick; 	  // The end of the time range of this bar
	firstTick = Bars[barno].getTime();
	lastTick = firstTick + Bars[barno].getTimeLen() - 1;

	unsigned long i;			// Counter

	//
	// Try to find value (deltaDen) of which all the notes' delta times
	// are EVENLY divisible. (time % deltaDen == 0)
	//

	short divRoundCounter;		// Search round counter for deltaDenUsed[], notesFound
	char allDivisible;			// (boolean) All notes are divisible by current deltaDen
	char divError;				// (boolean) Value deltaDen is not evenly divisible
	char tripletNext;			// (boolean) Next round(divRoundCounter+1) is with triplet times

	short deltaDen; 			// As result: MIDI delta time per one tabulature character
	short deltaLimit;			// Minimum value for deltaDen
	unsigned char trl;			// TabOptions::tripletlength()

	const unsigned long MAX_QUANTZ_ROUND = 10;
	short deltaDenUsed[MAX_QUANTZ_ROUND];
	short notesFound[MAX_QUANTZ_ROUND];
		// Results of the search rounds indexed by divRoundCounter, needed only when divError==1
		// MAX_QUANTZ_ROUND = number of different search rounds:
		// 0 = quarter notes, 1 = triplet quarters,
		// 2 = eight, 4 = 16th, 6 = 32nd, ...

	unsigned long time; 		// Note's time in ticks from the beginning of song
	unsigned long timeInBar;	// Note's time in ticks from the beginning of this bar[barno]
	unsigned char key;			// Just for Notes.getNote(...)

	// Examples of some values of deltaDen when (midiheader.getDiv() / options.minQ()) = 480
	// (4 / 2) * (480 / 120) = 2 * 4   = 8 (eigth chars per half note)	 (1/16 note)
	// (4 / 4) * (480 / 120) = 1 * 4   = 4 (four chars per quarter note) (1/16 note)
	// (4 / 8) * (480 / 120) = 0.5 * 4 = 2 (two chars per eigth note)	 (1/16 note)
	// (4 / 4) * (480 / 80)  = 1 * 6   = 6 (six chars per quarter note)  (1/16 with triplets)
	// (4 / 8) * (480 / 60)  = 0.5 * 8 = 4 (four chars per eigth note)	 (1/32 note)

	// Check options and set default deltaDen and deltaLimit
	trl = options.tripletlength();

	deltaDen = midiheader.getDiv() / options.minQ();

	// Set deltaLimit so that bars of which length
	// is greater than page width will not appear
	deltaLimit = 1;
	tripletNext = 1;
	while(Bars[barno].getTimeLen()/deltaLimit >
		  options.cols() - (options.drumnamelen() + 2))
	{
		if (tripletNext == 1)
		{
			deltaLimit *= 3;
			tripletNext = 0;
		}
		else
		{
			deltaLimit *= 2;
			tripletNext = 1;
		}
	}

	divRoundCounter = 0;
	allDivisible = 1;
	divError = 0;
	tripletNext = 1;

	do {
		deltaDenUsed[divRoundCounter] = deltaDen;
		notesFound[divRoundCounter] = 0;

		allDivisible = 1;
		i = firstnote;
		do {
			Notes.getNote(i, time, key);
			timeInBar = time - firstTick;

			if ((timeInBar % deltaDen) != 0)
			{
				allDivisible = 0;
			}
			else
			{
				notesFound[divRoundCounter]++;
			}
			i++;
		} while(i < (firstnote + notecount));

		if (allDivisible == 0)
		{
			// Decrease deltaDen to find notes that where not divisible on last round
			// Divide value by 2, 1.5 or 0.75 if next round is trying with triplets (tripletNext == 1)
			// Example: quarter notes -> triplet quarters -> eights -> triplet eights etc.

			if (tripletNext) {
				if (((deltaDen*4*trl)%(3*trl)) != 0)
				{
					divError = 1;
				}
				else {
					deltaDen = short((deltaDen * 4)/(3*trl));
					tripletNext = 0;
				}
			}
			else {
				if ((deltaDen*3*trl)%8 != 0)
				{
					divError = 1;
				}
				else {
					deltaDen = short((deltaDen * 3 * trl) / 8);
					tripletNext = 1;
				}
			}
		}

		if (deltaDen >= deltaLimit) divRoundCounter++;
		else divError = 1;

	} while(allDivisible == 0 && divError == 0 &&
		  divRoundCounter < MAX_QUANTZ_ROUND);

	//
	// Force quantization with best note match if previous search failed
	// (All the notes in Bars[barno] weren't divisible by same value)
	//

	if (divError == 1)
	{
		short besti = 0;		// Index of the most successful quantization round
		short best = 0; 		// Best number of divisible notes
		for (i=0; i < divRoundCounter; i++)
		{
			if (notesFound[i] > best)
			{
				best = notesFound[i];
				besti = i;
			}
		}

		deltaDen = deltaDenUsed[besti];
	}

	//
	// Quantize (divide note times by deltaDen) and add notes
	//

	vector<TabNote> quantizedNotes;
	quantizedNotes.reserve(notecount);
	TabNote note1;

	for (i = firstnote; i < (firstnote + notecount); i++) {
		Notes.getNote(i, time, note1.key);
		timeInBar = time - firstTick;
		note1.pos = (timeInBar - (timeInBar % deltaDen)) / deltaDen;
		quantizedNotes.push_back(note1);
	}

	//
	// Add bar with quantized tab notes to Bars
	//

	TabBar thisBar;
	thisBar = Bars[barno];
	thisBar.addNotes(quantizedNotes);
	thisBar.setCharPDen((4 / Bars[barno].getDen()) * (midiheader.getDiv() / deltaDen) );
	Bars2.push_back(thisBar);

	return divError;
}

short TabTrack::writetab(char * filename, char * titleText, TabOptions options)
{
	// Writes text-type drum tabulature to file (DOS format, UNIX compatible?)
	// returns 1 if success
	// if returns 0, check TabTrack.errDesc() for error details

	// Example of output:
	//
	// /-drum name							 Time signature is 4/4.
	//
	//	 /---- a bar -----/- another bar --\
	// H |x-x-x-x-x-x-x-x-|x-x-x-x-x-x-x-x-| <- hi-hat			\
	// S |----o-------o---|----o-------o---| <- snare drum		| One bar line
	// B |o-------o-o-----|o---------o-----| <- bass drum		|
	//	 |1 + 2 + 3 + 4 + |1 + 2 + 3 + 4 + | <- meter help line /
	//
	// H |x-x-x-x-x-x-x-x-|x---x---x-x-----|					\
	// S |----o-------o---|----o-------oooo|					| Another bar line
	// B |o-------o-o-----|o---------o-----|					|
	//	 |1 + 2 + 3 + 4 + |1 + 2 + 3 + 4e+a|					/
	//
	// Each drum type has its own line and every line has a specific positioning order
	// among other lines. Drum names and their order is gained from specified tab drum kit
	// in tab options (TabOptions * options).
	//
	// Options also include other settings like page size and usage of meter help line.

	if (!NoError) return 0;
	debugcout("Writetab");

	//
	// Exit straight if can't create output file to write tab
	//

	ofstream outfile;
	outfile.open(filename, ios::bin | ios::out);
	if (!outfile.good()) {
		NoError = 0;
		ErrorNum = 201;
		return 0;
	}

	debugcout("Output opened.");

	unsigned long i;				// Bar counter
	// this->barCount				// Maximum value for 'i'
	unsigned long firstBar; 		// First bar in current row
	unsigned long j;				// Another counter

	unsigned char pagenum = options.pagenum();
	unsigned char pageEnds = 0; 	// Boolean (if pagenum): no more bars can fit on current page
	unsigned short rowCount = 0;	// Counter needed by when using page height limit (in characters)
	unsigned short colCount = 0;	// Counter needed by page width limit (in characters)
	unsigned short pageCount = 1;	// Page number
	char onlyPage = 0;				// Boolean: highest page number will be 1,
									//			don't print page number

//	   unsigned char newPage = 0;
//	   unsigned char fitError = 0;	  // Boolean

	char meterline; 				// Boolean: use |1 + 2 + 3 + 4 + | help line below bars?
	char allempty;					// Boolean: current row has no notes (drumLines == 0)
	char infoline;					// How many lines above barline; usually 0 or 1 for
									// info about meter changes and such
	unsigned short rowsAfter = 0;	// Empty rows between two bar rows

	char * usedKeys;				// Booleans: different note key values (drum types) used
	char * ignoredKeys; 			// Booleans: key is not supported by drumkit -> ignored
	char keysIgnored;				// Boolean: Keys ignored?
	unsigned char allKeys;			// Current number of all diffent keys will be shown
	unsigned char drumLines;		// Height of current bar in rows (number of drum lines)
	unsigned char * lineKeys;		// Master keys that will be used on drum lines
	short * usedKeysPos;			// Vertical position of key (drum) in tab
	char * lineInUse;				// Booleans: every key having a line number (order)
									// on where it's marked, this contains information about
									// which lines are in use.

	unsigned char * sortedKey;		// When sorting different note key values to a specific order
	short * sortedPos;

	char ch;

	TabDrumkit drumkit;
	drumkit = *(options.drumkit());

	char * infoText;				// Buffer for info line above bar line
	char * outbars; 				// Buffer for current bar line that will be written to file
	char * meterlineText;			// Buffer for meter line below bar line
	char * pageText;				// Buffer for page numbers

	//
	// CHECK OPTIONS
	//

	// infoline will be checked in writeInfoline()
	meterline = options.meterline();
	rowsAfter = options.rowsbetwbars();

	// DEBUG/TEST: IGNORE METERLINE
	meterline = 0;

	//
	// SOME PRE-ALLOCATIONS
	//

	usedKeys = new char[128];
	ignoredKeys = new char[128];
	usedKeysPos = new short[128];
	lineInUse = new char[drumkit.maxorder()];
	lineKeys = new unsigned char[drumkit.maxorder()];

	sortedKey = new unsigned char[128];
	sortedPos = new short[128];

	if (usedKeys	== NULL || usedKeysPos == NULL || lineInUse == NULL ||
	   lineKeys    == NULL || ignoredKeys == NULL || sortedKey == NULL ||
	   sortedPos == NULL)
	{
		NoError = 0;
		ErrorNum = 1;

		delete [] usedKeys;
		delete [] ignoredKeys;
		delete [] usedKeysPos;
		delete [] lineInUse;
		delete [] lineKeys;
		delete [] sortedKey;
		delete [] sortedPos;

		return 0;
	}

	if(options.userows() == 1)
	{
		pageText = new char[options.rows() + 1];
		if(pageText == NULL)
		{
			NoError = 0;
			ErrorNum = 1;

			delete [] usedKeys;
			delete [] ignoredKeys;
			delete [] usedKeysPos;
			delete [] lineInUse;
			delete [] lineKeys;
			delete [] sortedKey;
			delete [] sortedPos;

			return 0;
		}
		else pageText[options.rows()] = '\0';
	}

	//
	// WRITE LOOP BEGINS
	//

	// Write title if such exists

	// if(titleText != NULL)
	if(1==2)
	{
		j = options.rowsbeforetitle();
		for(i = 0; i < j; i++)
		{
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
		}

		j = strlen(titleText);
		if(j > options.cols()) j = options.cols();

		if(options.titlealign()==1 && j < options.cols())
		{
			// Title aligned to center of page
			ch = ' ';
			for(i = 0; i < int((options.cols()-j)/2); i++)
				outfile.write(&ch, 1);
		}

		if(options.titlealign()==1 && j < options.cols())
		{
			// Title aligned to the right margin
			ch = ' ';
			for(i = 0; i < options.cols()-j; i++)
				outfile.write(&ch, 1);
		}

		outfile.write(titleText, j);
		ch = 13; outfile.write(&ch, 1); // EOL
		ch = 10; outfile.write(&ch, 1);

		j = options.rowsfortitle() - options.rowsbeforetitle();
		for (i = 0; i < j; i++)
		{
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
		}
		rowCount = options.rowsfortitle();
	}

	i = 0;					// Bar counter
	firstBar;				// Range [firstBar, i] = bars currently written

	while(i < BarCount)
	{
		// Define bars to print at this round
		// (How many bars can be fitted on the current row?)

												// colCount = currently reserved characters on row
		colCount = options.drumnamelen() + 1;	// drumnamelen = space for drum symbol
												// 1 = first barline, marked as a '|'
		firstBar = i;
		do {

			// Bars2[i].getCharLen() = character length of the next bar
			// 1					 = vertical end line for bar, marked as a '|'
			if ((colCount + Bars2[i].getCharLen() + 1) <= options.cols())
			{
				// Bar [i] will be written at this round.
				// Add the space it needs to colCount.

				colCount += Bars2[i].getCharLen() + 1;
				i++;
			}
		} while(i < BarCount &&
			   (colCount + Bars2[i].getCharLen() + 1) < options.cols());

		i--;	// 'i' is the index of last bar in current range

		// Unable to find new bars that fit on page?
		// ('i' was not increased)

		if (firstBar > i && firstBar < BarCount-1)
		{
			// Bar length is greater than page width
			// Bars2[x].getCharLen() + options.drumnamelen() > options.cols()
			NoError = 0;
			ErrorNum = 202;
			ErrorRef = i;

			delete [] usedKeys;
			delete [] ignoredKeys;
			delete [] usedKeysPos;
			delete [] lineInUse;
			delete [] lineKeys;
			delete [] sortedKey;
			delete [] sortedPos;

			delete [] infoText;
			delete [] outbars;
			delete [] meterlineText;
			delete [] pageText;

			return 0;
		}

		// Range of bars to print at this round is now [firstBar to i]
/*		  dbcout("Bars: [" << firstBar << "," << i << "]" << endl);
*/
		// Define height of bars in lines:
		// Analyze notes and calculate amount of key values
		// (=different drums) that need an own line for their notes

		// Get different key values of notes used in every bar
		for (j = 0; j < 128; j++) usedKeys[j] = 0;
		for (j = firstBar; j < i + 1; j++) Bars2[j].getUsed(usedKeys);

		keysIgnored = checkKeys(allKeys, drumLines, usedKeys,
								ignoredKeys, lineInUse, lineKeys, drumkit);

		if (keysIgnored == 1) printIgnoredKeys(ignoredKeys, firstBar, i);

		// Info line (allocates and writes when necessary)
		// infoline = writeInfoline(infoText, options, colCount, firstBar, i);
		// dbcout("infoline OK." << endl);
		infoline = 0;

		// Allocate memory for text output = outbars (current tab row)
		//									 and meterlineText
		// (colCount + 2)			   width of text buffer (+EOL mark)
		// (drumLines + meterline)	 height of text buffer
		// rowsAfter * 2			   2 bytes for each EOL (end-of-line) mark

		if (allKeys == 0)
		{
			// If all bars in current row are empty (no notes),
			// mark them like '    |-----------------|-----------------|---...'
			outbars = new char[colCount + 2];
			allempty = 1;
		}
		else
		{
			outbars = new char[(colCount + 2) * drumLines];
			if (meterline != 0) meterlineText = new char[colCount + 2];
			allempty = 0;
		}
		if (outbars == NULL || (meterline != 0 && meterlineText == NULL))
		{
			NoError = 0;
			ErrorNum = 1;

			delete [] usedKeys;
			delete [] ignoredKeys;
			delete [] usedKeysPos;
			delete [] lineInUse;
			delete [] lineKeys;
			delete [] sortedKey;
			delete [] sortedPos;

			delete [] infoText;
			delete [] outbars;
			delete [] meterlineText;
			delete [] pageText;

			return 0;
		}

		if (allKeys > 0)
		{
			sortKeys(allKeys, drumLines, usedKeys, usedKeysPos,
					 drumkit, sortedKey, sortedPos, lineKeys);
		}

		formatBars(outbars, drumkit, drumLines, lineKeys,
				   colCount, options.drumnamelen(), firstBar, i);

		if (allKeys > 0)
		{
			if (meterline) {
				// Format meterlineText for writeNotes
				for (j = 0; j < colCount; j++) meterlineText[j] = 0;
				meterlineText[colCount] 	= 13; // EOL
				meterlineText[colCount + 1] = 10;
			}
			writeNotes(outbars, meterlineText, colCount,
					   drumkit, options, usedKeys,
					   usedKeysPos, drumLines,
					   firstBar, i);
		}

		// Meter line
		if (meterline != 0)
		{
			meterlineText = new char[colCount + 2];
			if (meterlineText == NULL) {
				NoError = 0;
				ErrorNum = 1;

				delete [] usedKeys;
				delete [] ignoredKeys;
				delete [] usedKeysPos;
				delete [] lineInUse;
				delete [] lineKeys;
				delete [] sortedKey;
				delete [] sortedPos;

				delete [] infoText;
				delete [] outbars;
				delete [] meterlineText;
				delete [] pageText;

				return 0;
			}
			else writeMeterline(meterlineText, colCount, firstBar, i, meterline);
		}

		// 'Lines per page' -limit (
		if (options.userows() == 1)
		{
			if (i+1 == BarCount && pagenum == 1 && pageCount == 1)
			{
				// If this bar line can be fitted on the current page,
				// there is no need for page numbering
				// because highest page number would be 1.
				onlyPage = 1;
			}

			// Bar line does not fit on this page with standard value of rowsAfter?
			if ((rowCount + infoline + drumLines + meterline + rowsAfter + pagenum) > options.rows())
			{
				// Bar line does not fit on this page?
				if ((rowCount + infoline + drumLines + meterline + pagenum) > options.rows())
				{
					if (onlyPage == 1) onlyPage = 0;
				}
				else
				{
					// This bar line (Bars2[firstBar] to Bars2[i]) is
					// the last bar line on current page (pageCount).
					// Just decrease rowsAfter so that bar fits on this page.
					rowsAfter = options.rows() - (rowCount + infoline + drumLines + meterline + pagenum);
				}
				pageEnds = 1;
			}

		}

		// End of current page?
		if (pageEnds == 1 && onlyPage != 1)
		{
			// Fill the rest of the page (-pagenum) with empty lines if required
			// so the page size in lines will be same as options.rows()
			for (j = 0; j < (options.rows() - (rowCount + pagenum)); j++)
			{
				ch = 13; outfile.write(&ch, 1); // EOL
				ch = 10; outfile.write(&ch, 1);
			}

			if (pagenum == 1)
			{
				writePageNumbers(pageText, pageCount, filename,
								 titleText, options.cols());

				if (pageText != NULL)
				{
					if (pageText != NULL)
					{
						outfile.write(pageText, options.cols());
						dbcout(pageText << endl);
					}
				}

				ch = 13; outfile.write(&ch, 1); // EOL
				ch = 10; outfile.write(&ch, 1);
				dbcout(endl);
				pageCount++;
			}
			pageEnds = 0;
			rowCount = 0;
			rowsAfter = options.rowsbetwbars();
		}

		// Write bar(s) to file
		if (infoline)
		{
			outfile.write(infoText, (colCount + 2) * infoline);
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
			rowCount+=infoline;
			dbcout("infoline" << endl);
		}

		outfile.write(outbars, (colCount + 2) * (drumLines + allempty));
		rowCount+=(drumLines + allempty);
		for (j=0; j < (colCount + 2) * (drumLines + allempty) - 2; j++)
		{
			dbcout(outbars[j]);
		}

		if (meterline)
		{
			outfile.write(meterlineText, colCount + 2);
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
			rowCount++;
			dbcout("meterline" << endl);
		}

		for (j = 0; j < rowsAfter; j++)
		{
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
			rowCount++;
			dbcout(endl);
		}
/*		  dbcout("write OK." << endl);
*/
		delete [] outbars;
		delete [] meterlineText;
		i++; // i changes to the index of first bar in next range

	} // while(i < BarCount)

	// Page numbers on the last page

	// If the whole tab has only one page and
	// song info (name, author etc.) was not printed
	// on the first page, print the filename without
	// extension and page number to the bottom of the page

	if (options.userows() == 1 && rowCount < options.rows() && pagenum == 1 ||
		(onlyPage == 1 && titleText == NULL))
	{
		for (j = 0; j < (options.rows() - (rowCount + pagenum)); j++)
		{
			ch = 13; outfile.write(&ch, 1); // EOL
			ch = 10; outfile.write(&ch, 1);
		}

		writePageNumbers(pageText, pageCount, filename, 0, options.cols());
		if (pageText != NULL)
		{
			outfile.write(pageText, options.cols());
		}

		// No EOL mark will be written, because it may cause an extra
		// empty page when finally printing the tab.

	}

	debugcout("OK. (now closing the file)");

	outfile.close();

	// Free memory
	delete [] usedKeys;
	delete [] ignoredKeys;
	delete [] usedKeysPos;
	delete [] lineInUse;
	delete [] lineKeys;
	delete [] sortedKey;
	delete [] sortedPos;

	delete [] infoText;
	delete [] outbars;
	delete [] meterlineText;
	delete [] pageText;

	// Tab writed successfully.
	return 1;

}

char TabTrack::checkKeys(unsigned char & allKeys, unsigned char & drumLines,
	 char * usedKeys, char * ignoredKeys, char * lineInUse, unsigned char * lineKeys,
	 TabDrumkit & drumkit)
{
	// Check key values that the notes in current range of bars have
	// - Will the key need an own line in tab?
	//	 (=is key master or slave, see "drumkit.hpp")
	// - Is key invalid = not defined in drumkit?

	// Explanations for all parametre variables can be found
	// at the beginning of TabTrack::writetab()

	short i;
	char keysIgnored = 0;
	allKeys = 0;
	drumLines = 0;

	for (i = 0; i < drumkit.maxorder(); i++)
	{
		lineInUse[i] = 0;
	}

	for (i = 0; i < 128; i++)	 // Key values 0 to 127
	{
		ignoredKeys[i] = 0;

		if (usedKeys[i] == 1)
		{
			if (drumkit.order(i) == -1)
			{
				// Order is -1.
				// Key is not defined in drumkit
				// and therefore it will be ignored

				usedKeys[i] = 0;
				ignoredKeys[i] = 1;
				keysIgnored = 1;
			}
			else {

				if (drumkit.base(i) == -1)
				{
					// Key is a 'master' key; it has own line

					allKeys++;
					if (lineInUse[drumkit.order(i)] == 0)
					{
						lineInUse[drumkit.order(i)] = 1;
						lineKeys[drumLines] = i;
						drumLines++;
					}
				}
				else {

					// Key is a 'slave' key; it is marked
					// on line of a master key,

					if (lineInUse[drumkit.order(drumkit.base(i))] == 0)
					{
						// Line is not reserved (master key or other keys
						// having same master are not in use)
						lineInUse[drumkit.order(drumkit.base(i))] = 1;
						lineKeys[drumLines] = drumkit.base(i);
						allKeys++;
						drumLines++;
					}
					else {
						// The master key of current key is in use
						// and a line is reserved for it.
						// This key will use the same line as the master key.
						// Don't increase drumLines.
						allKeys++;
					}
				} // Slave? if (drumkit.order(base) == -1)
			} // Master? if (base != -1)
		} // In use? if (drumkit.order() != -1)
	} // for (i)

	return keysIgnored;
}

void TabTrack::printIgnoredKeys(char * ignoredKeys,
	unsigned long firstBar, unsigned long lastBar) const
{
	// Lists from range of bars notes that are not supported by drumkit
	cout << "Unknown keys in bars [" << firstBar+1 << "," <<
			lastBar+1 << "]:";

	short octave;
	short keyname;

	for (short i = 0; i < 128; i++)
	{
		if (ignoredKeys[i] == 1)
		{
			cout << " ";

			// Key name
			keyname = i%12;
			octave = (i-keyname)/12;

			switch(keyname)
			{
			case 0:  cout << "C"; break;
			case 1:  cout << "C#"; break;
			case 2:  cout << "D"; break;
			case 3:  cout << "D#"; break;
			case 4:  cout << "E"; break;
			case 5:  cout << "F"; break;
			case 6:  cout << "F#"; break;
			case 7:  cout << "G"; break;
			case 8:  cout << "G#"; break;
			case 9:  cout << "A"; break;
			case 10: cout << "A#"; break;
			case 11: cout << "B"; break;
			}
			cout << octave;

			// Key number
			cout << " <" << i << ">";
		}
	}
	cout << endl;
}

void TabTrack::sortKeys(unsigned char allKeys, unsigned char drumLines,
			   char * usedKeys, short * usedKeysPos, TabDrumkit & drumkit,
			   unsigned char * sortedKey, short * sortedPos,
			   unsigned char * lineKeys)
{
	// This function is called from TabTrack::write()
	// It gets an vertical positioning order for all used keys.
	// Positioning for different key values is gained from
	// drumkit

	short i;
	short j;
	short k;
	// Get different positions for different keys
	// from the drumkit
	for (i = 0; i < 128; i++)
	{
		sortedKey[i] = 0;
		sortedPos[i] = -1;
	}

	k = 0;
	for (i = 0; i < drumkit.maxorder(); i++) {
		for (j = 0; j < 128; j++) {
			if (usedKeys[j] == 1) {
				if (drumkit.order(j) == i) {
					sortedKey[k] = j;	 // Key value
					sortedPos[k] = i;	 // Vertical position (=order number)
					k++;
				}
			}
		}
	}

	// Organize values of sortedPos[] to go 0,1,2,3, ...

	short h = -1;					// Highest position value in use
	short l = drumkit.maxorder();	// Lowest pos. value that hasn't been changed

	for (i = 0; i < allKeys; i++)
	{
		// Find lowest value not in use (l)
		for (j = 0; j < allKeys; j++)
		{
			if (sortedPos[j] < l && sortedPos[j] > h) l = sortedPos[j];
		}

		// Change keys having positions l to lowest possible position (h)
		for (j = 0; j < allKeys; j++)
		{
			if (sortedPos[j] == l) sortedPos[j] = h+1;
		}

		h++;
		l = drumkit.maxorder();
		// (It must be assumed that on every round of i
		//	some values will be decreased so that h may be increased.)
	}

	// Create usedKeysPos (for note positioning)
	for (i = 0; i < allKeys; i++)
	{
		usedKeysPos[ sortedKey[i] ] = sortedPos[i];
	}

	// Sort lineKeys (for drum symbol positioning)
	unsigned char tmp;
	for (i = 0; i < drumLines; i++)
	{
		for (j = drumLines - 1; j > i; j--)
		{
			if (drumkit.order(lineKeys[j]) < drumkit.order(lineKeys[j-1]))
			{
				tmp = lineKeys[j];
				lineKeys[j] = lineKeys[j-1];
				lineKeys[j-1] = tmp;
			}
		}
	}
}

void TabTrack::formatBars(char * outbars, TabDrumkit & drumkit, unsigned char drumLines,
					unsigned char * lineKeys,
					unsigned short colCount, unsigned char drumnamelen,
					unsigned long firstBar, unsigned long lastBar)
{
	// Formats a new, empty tabulature with drum lines
	// to buffer (outbars)

	unsigned long i;
	short j;
	char * drumname; // pointer to string for current drum symbol
	short txtOffset;
	unsigned short l;

	// Format base tabulature
	for (i = 0; i < drumLines; i++)
	{
		// Set drum name  (sortedPos[] is finally used here)
		drumname = drumkit.shortName(lineKeys[i], drumnamelen);

		for (j = 0; j < drumnamelen; j++)
		{
			outbars[(drumLines - 1 - i) * (colCount + 2) + j] = drumname[j];
		}

		// First vertical bar separator markers (also known as barlines)
		outbars[i * (colCount + 2) + drumnamelen] = '|';

		// Fill bars with '----' and after them, a '|'
		// txtOffset = beginning point for '-' marks
		// example: 'HH  |------- ...' <- txtOffset = 4 + 1 = 5
		txtOffset = drumnamelen + 1;
		for (j = firstBar; j < lastBar + 1; j++)
		{
			// Bar length in characters = numerator * characters per denominator
			for (l = 0; l < Bars2[j].getCharLen(); l++)
			{
				outbars[i * (colCount + 2) + txtOffset + l] = '-';
			}
			outbars[i * (colCount + 2) + txtOffset + l] = '|';
			txtOffset += l + 1; // add '... ------|' for the next bar
		}

		// End of line (two bytes)

		outbars[(i+1) * (colCount + 2) - 2] = 13; // EOL
		outbars[(i+1) * (colCount + 2) - 1] = 10;
	}

	// If all bars in current row are empty (no notes),
	// mark them like '    |-----------------|-----------------|---...'
	if (drumLines == 0)
	{
		for (i = 0; i < drumnamelen; i++) outbars[i] = ' ';
		outbars[drumnamelen] = '|';
		txtOffset = drumnamelen + 1;
		for (i = firstBar; i < lastBar + 1; i++)
		{
			// Bar length in characters = numerator * characters per denominator
			for (j = 0; j < Bars2[i].getCharLen(); j++)
			{
				outbars[txtOffset + j] = '-';
			}
			txtOffset += j;
			outbars[txtOffset] = '|';
			txtOffset += 1;
		}
		outbars[txtOffset]	   = 13; // EOL
		outbars[txtOffset + 1] = 10;
	}
}

void TabTrack::writeNotes(char * outbars, char * meterlineText,
	 unsigned short colCount, TabDrumkit & drumkit, const TabOptions & options,
	 char * usedKeys, short * usedKeysPos, unsigned char drumLines,
	 unsigned long firstBar, unsigned long lastBar)
{
	// Writes tab notes to pre-formatted buffer (outbars)
	// Bars2 contains quantized notes.

	short txtOffset;
	short i;
	short j;
	short k;
	TabNote note;

	txtOffset = options.drumnamelen() + 1;
	for (i = firstBar; i < lastBar + 1; i++)
	{
		for (j = 0; j < Bars2[i].getNoteCount(); j++)
		{
			note = Bars2[i].getNote(j);
			if (usedKeys[note.key] == 1)
			{
				outbars[ (drumLines - 1 - usedKeysPos[note.key]) *
						 (colCount + 2) + txtOffset + note.pos ] =
						 drumkit.mark(note.key);

				// In case of automatic meter line, mark the positions of notes
				if (options.meterline() == 2) meterlineText[txtOffset + note.pos] = 1;
			}
		}
		txtOffset += (Bars2[i].getNum() * Bars2[i].getCharPDen()) + 1;
	}
}

char TabTrack::writeInfoline(char * infoText, const TabOptions & options,
				   unsigned short colCount, unsigned long firstBar, unsigned long lastBar)
{
	// Writes info lines above bar line when necessary.
	// These lines may contain information about:
	// 1. meter changes

	short i;
	short j;
	char linecount1 = 0;
	char linecount = 0;

	// Check options and allocate the buffer
	if (options.meterchanges() == 1) linecount1++;
	delete [] infoText;
	infoText = new char[(colCount + 2) * linecount1];

	if (infoText == NULL)
	{
		NoError = 0;
		ErrorNum = 1;
		return 0;
	}

	// Format with space characters and EOLs
	for (i = 0; i < linecount1; i++) {
		for (j = 0; j < colCount; j++)
			infoText[(colCount + 2) * i + j] = ' ';
		infoText[(colCount + 2) * (i + 1) - 2] = 13; // EOL
		infoText[(colCount + 2) * (i + 1) - 1] = 10;
	}

	// 1. Meter changes
	if (options.meterchanges() == 1)
	{
		char mchange = 0;
		char numerator[sizeof(unsigned long)*8+1];
		char denominator[sizeof(unsigned long)*8+1];
		unsigned short offset;	// Characters from the left

		for (i = firstBar; i < lastBar + 1; i++) {

			if (firstBar == 0)
			{
				mchange = 1;
				offset = options.drumnamelen() + 1;
			}
			else {
				if (Bars2[i-1].getNum() != Bars2[i].getNum() ||
				   Bars2[i-1].getDen() != Bars2[i].getDen())
				{
					mchange = 1;
				}
				else
				{
					mchange = 0;
				}
			}
			if (mchange) {
				_ultoa(Bars2[i].getNum(), numerator, 10);
				_ultoa(Bars2[i].getDen(), denominator, 10);

				for (j = 0; j < strlen(numerator); j++) {
					if (offset + j < colCount)
					infoText[offset + j] = numerator[j];
				}

				if (offset + j < colCount)
					infoText[offset + j] = '/';

				for (j = 0; j < strlen(denominator); j++) {
					if (offset + strlen(numerator) + 1 + j < colCount)
					infoText[offset + strlen(numerator) + 1 + j] = denominator[j];
				}
				offset += Bars2[i].getCharLen() + 1;
			}

		}
		linecount++;
	}

	return linecount;
}

void TabTrack::writeMeterline(char * meterlineText, unsigned short colCount,
				   unsigned long firstBar, unsigned long lastBar,
				   char meterline)
{
}

/*
{
		short cpd;
		char ch;

		// Write meter line
		if (options.meterline())
		{
			// Format line
//			  for (txtOffset = 0; txtOffset < options.drumnamelen(); txtOffset++)
//				   outbars[drumLines * (options.cols() + 2) + txtOffset] = ' ';
//			   outbars[drumLines * (options.cols() + 2) + txtOffset] = '|';
//
//			   for (; txtOffset<options.cols(); txtOffset++)
//				   outbars[drumLines * (options.cols() + 2) + txtOffset] = ' ';

			for (txtOffset = 0; txtOffset < options.drumnamelen(); txtOffset++)
				outbars[drumLines * (colCount + 2) + txtOffset] = ' ';
			outbars[drumLines * (colCount + 2) + txtOffset] = '|';

			for (; txtOffset<options.cols(); txtOffset++)
				outbars[drumLines * (options.cols() + 2) + txtOffset] = ' ';

			outbars[drumLines * (colCount + 2) + txtOffset - 2] = 13; // EOL
			outbars[drumLines * (colCount + 2) + txtOffset - 1] = 10;

			// Marks
			txtOffset = options.drumnamelen() + 1;
			for (j=firstBar; j<i; j++)
			{
				cpd = Bars2[k].getCharPDen();
				for (k=0, l=1; k<Bars2[j].getNum(); k++)
				{
					o = k * cpd;
					switch(l)
					{
						case 1: { ch = '1'; break; }
						case 2: { ch = '2'; break; }
						case 3: { ch = '3'; break; }
						case 4: { ch = '4'; break; }
						case 5: { ch = '5'; break; }
						case 6: { ch = '6'; break; }
						case 7: { ch = '7'; break; }
						case 8: { ch = '8'; break; }
						case 9: { ch = '9'; break; }
						case 0: { ch = '0'; break; }
					}

					// 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, ...
					outbars[drumLines * (options.cols() + 2) + txtOffset + o] = ch;

					// +
					if (cpd >= 2 && cpd%3==0)
					{
						outbars[drumLines * (options.cols() + 2) + txtOffset + o + (cpd/2)] = '+';
					}

					// e, a
					if (cpd >= 4 && cpd%3==0)
					{
						// Always
						if (options.meterline()==1)
						{
							outbars[drumLines * (options.cols() + 2) + txtOffset + o + (cpd/4)*1] = 'e';
							outbars[drumLines * (options.cols() + 2) + txtOffset + o + (cpd/4)*3] = 'a';
						}

						// Automatic (only when note at same time)
						if (options.meterline()==2)
						{
							if (meterlineText[txtOffset + o + (cpd/4)*1]==1)
							  outbars[drumLines * (options.cols() + 2) + txtOffset + o + (cpd/4)*1] = 'e';
							if (meterlineText[txtOffset + o + (cpd/4)*3]==1)
							outbars[drumLines * (options.cols() + 2) + txtOffset + o + (cpd/4)*3] = 'a';
						}
					}

					l++;
					if (l==10) l=0;
				}
				txtOffset+=(Bars2[j].getNum() * cpd);
				outbars[drumLines * (options.cols() + 2) + txtOffset] = '|';
				txtOffset++;
			}
		}
		debugcout("007 (meter line written?)");

		// Empty lines after bar
		for (j=0; j<rowsAfter; j++)
		{
			outbars[(drumLines + meterline) * (colCount + 2) + j * 2] = 13; // EOL
			outbars[(drumLines + meterline) * (colCount + 2) + j * 2 + 1] = 10;
		}
		debugcout("008 (rowsAfter)");
}
*/

void TabTrack::writePageNumbers(char * pageText, unsigned short pageCount,
					  char * filename, char * titlename, unsigned short cols)
{
	// Page text consists of filename without extension
	// and a page number. It is aligned to right.
	// Sample: (file test.mid, page 6)
	// left margin->					   test 6<-right margin

	// assert(filename != NULL && strlen(filename) >= 1);
	// Should be true if this function is called.

	char * pageStr;
	pageStr = new char [sizeof(unsigned long)*8+1];

	if (pageStr == NULL || pageText == NULL)
	{
		delete [] pageStr;
		NoError = 0;
		ErrorNum = 1;
	}
	else
	{
		unsigned short i;
		unsigned short flen;
		unsigned short nlen;
		for (i = 0; i < sizeof(unsigned long)*8+1; i++) pageStr[i] = ' ';
		for (i = 0; i < cols + 1; i++) pageText[i] = ' ';

		// Convert page number to string (pageStr)
		// and calculate its length (nlen)
		_ultoa((unsigned long)pageCount, pageStr, 10);
		nlen = strlen(pageStr);

		// Analyze filename or titlename to find '.mid' extension
		// i = length of filename substring in pageStr
		if(titlename == NULL)
		{
			flen = strlen(filename)-1;
			while(filename[flen] != '.' && flen > 0) flen--;
			if(flen == 0)
				flen = strlen(filename);
		}
		else
		{
			flen = strlen(titlename)-1;
			while(titlename[flen] != '.' && flen > 0) flen--;
			if(flen == 0)
				flen = strlen(titlename);
		}

		if(flen > cols-(nlen+1))
			flen = cols-(nlen+1);

		// Insert filename/titlename and pageStr to pageText
		for(i = 0; i < flen; i++)
		{
			if(titlename == NULL)
			{
				pageText[cols-(flen+1+nlen)+i] = filename[i];
			}
			else
			{
				pageText[cols-(flen+1+nlen)+i] = titlename[i];
			}
		}

		for(i = 0; i < nlen; i++)
		{
			pageText[cols-nlen+i] = pageStr[i];
		}
	}
}

void TabTrack::testprintQuantz()
{
	// Test/debug: prints contents of Bars2

	// Tests TabBar::getUsed()

	TabBar bar;
	TabNote note;
	char * usedkeys;
	usedkeys = new char[128];

	if (usedkeys != 0) {

		cout << "testprintQuantz()" << endl;

		short i;
		short j;

		for (j = 0; j < 128; j++) usedkeys[j] = 0;

		for (i = 0; i < Bars2.size(); i++)
		{
			bar = Bars2[i];

			// Print bar number and different key values
			cout << "Bar " << i << ": all keys={";

			// for (j = 0; j < 128; j++) usedkeys[j] = 0;
			bar.getUsed(usedkeys);

			for (j = 0; j < 128; j++)
			{
				if (usedkeys[j] == 1) cout << " " << j;
			}
			cout << "}" << endl;

			// Print notes
			for (j = 0; j < bar.getNoteCount(); j++)
			{
				note = bar.getNote(j);
				cout << "(" << note.pos << ")[" << short(note.key) << "] ";
			}
			cout << endl;
		}
	}
}

#endif
