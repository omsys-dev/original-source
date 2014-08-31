// mxl.h is the header for the mxl.exe MicroXML parser driver


int main(int argc, char* argv[]);


// help

void WriteHelp(bool api = false);


// JSON writing

void WriteJSON(element *el, long lev);
void Put32StrJSON(unl *u32arr, long cnt);


// utility functions

void PutTabs(long cnt);


// end of mxl.h

