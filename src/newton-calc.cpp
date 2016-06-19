/* Federico Poli Dicembre 2010 */
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// includi windows.h, aggiungi chiamate particolari di Ogre, ecc
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "json/json.h"

#include "basic.h"
#include "simulator.h"

void usage(const char *prog)
{
	printf("Usare %s [configurazione] [durata] [output]\n", prog);
	printf("  configurazione: percorso del file di configurazione\n");
	printf("  durata: durata della simulazione in secondi (anche non interi)\n");
	printf("  output: percorso del file in cui verrà salvata la configurazione finale\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	Real goal_time = 1; // secondi di simulazione
	Json::Value conf;
	char config_file_name[512] = "config.conf";
	char output_file_name[512] = "\0";

	// ======= Classi =======
	Simulator simulator; // Simulator

	// ======= Leggi il file di configurazione =======
	if (argc <= 1) {
		usage(argv[0]);
	}
	if (argc > 1) snprintf(config_file_name, sizeof(config_file_name), "%s", argv[1]);
	if (argc > 2) goal_time = atof(argv[2]);
	if (argc > 3) snprintf(output_file_name, sizeof(output_file_name), "%s", argv[3]);
	printf("Leggo il file di configurazione '%s'...\n", config_file_name);
	std::ifstream config_file;
	config_file.open(config_file_name);
	Json::Reader reader;
	if ( !reader.parse( config_file, conf ) ) {
		// report to the user the failure and their locations in the document.
		error("Errore nel leggere il file di configurazione:\n%s", reader.getFormatedErrorMessages().c_str());
	}
	config_file.close();

	// ======= Applica la configurazione =======
	simulator.load_configuration(conf);
	simulator.init_state();

	// ======= Start =======
	while (simulator.get_time() < goal_time) {
		simulator.evolve();
	}

	// ======= Scrivi il file con la nuova configurazione =======
	if (output_file_name[0] != '\0') {
		simulator.update_configuration(conf);
		printf("Salvo il file di configurazione '%s'...\n", output_file_name);
		std::ofstream output_file;
		output_file.open(output_file_name);
		Json::StyledWriter writer;
		output_file << writer.write(conf);
		config_file.close();
	}

	return 0;
}
