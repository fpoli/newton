/* Federico Poli Dicembre 2010 */
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// includi windows.h, aggiungi chiamate particolari di Ogre, ecc
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "ogre_app.h"

#include "json/json.h"

#include "basic.h"
#include "simulator.h"


void usage(const char *prog)
{
    printf("Per specificare un file di configurazione usare %s [percorso del file di configurazione]\n\n", prog);
}

int main(int argc, char *argv[])
{
    // ======= Classi =======
    Simulator simulator; // Simulator
    OgreApp ogreapp(&simulator); // Ogre Application

    // ======= Leggi il file di configurazione =======
    Json::Value conf;
    char config_file_name[512] = "config.conf\0";
    if (argc > 1) {
        snprintf(config_file_name, sizeof(config_file_name), "%s", argv[1]);
    } else {
        usage(argv[0]);
    }
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
    ogreapp.load_configuration(conf);
    
    // ======= Start =======
    ogreapp.startMainLoop();
    
    return 0;
}
