// config.h (para teste NFC)

#ifndef CONFIG_H
#define CONFIG_H

// --- SELECIONE O PROJETO ATIVO ---
#define PROJECT_ID_MEMENTO_MORI 1
#define ACTIVE_PROJECT_ID PROJECT_ID_MEMENTO_MORI

// --- FEATURES ADICIONAIS ---
#if (ACTIVE_PROJECT_ID == PROJECT_ID_MEMENTO_MORI)
  #define FEATURE_NFC_ENABLED 1
#endif

// --- Definições de Hardware ---
#if defined(FEATURE_NFC_ENABLED)
  // Define a interface para a biblioteca PN532
  #define PN532_INTERFACE_I2C
#endif

#endif // CONFIG_H