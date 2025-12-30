#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Etudiant.h"


// ==================== FONCTIONS DE COMPARAISON ====================

// Fonction de comparaison pour le tri par nom
int comparerParNom(const Etudiant *a, const Etudiant *b) {
    int cmpNom = strcasecmp(a->nom, b->nom);
    if (cmpNom != 0) return cmpNom;
    return strcasecmp(a->prenom, b->prenom);
}

// Fonction de comparaison pour le tri par matricule
int comparerParMatricule(const Etudiant *a, const Etudiant *b) {
    return strcmp(a->matricule, b->matricule);
}

// Fonction de comparaison pour le tri par région
int comparerParRegion(const Etudiant *a, const Etudiant *b) {
    int cmpRegion = strcasecmp(a->region, b->region);
    if (cmpRegion != 0) return cmpRegion;
    return comparerParNom(a, b);
}

// Fonction de comparaison pour le tri par filière
int comparerParFiliere(const Etudiant *a, const Etudiant *b) {
    int cmpFiliere = strcasecmp(a->filiere, b->filiere);
    if (cmpFiliere != 0) return cmpFiliere;
    return comparerParNom(a, b);
}

// ==================== FONCTION GÉNÉRIQUE DE TRI ====================

// Fonction générique pour lire, trier et afficher les étudiants
static void trierEtAfficher(const char *nomFichier, 
                          int (*comparer)(const Etudiant*, const Etudiant*), 
                          const char *critere) {
    // 1. Obtenir le nombre d'étudiants
    int n = nombreEtudiant(nomFichier);
    if (n == 0) {
        printf("Aucun etudiant a trier.\n");
        return;
    }
    
    // 2. Ouvrir le fichier
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }
    
    // 3. Allouer un tableau pour stocker les étudiants
    Etudiant *etudiants = malloc(n * sizeof(Etudiant));
    if (etudiants == NULL) {
        printf("Erreur d'allocation memoire.\n");
        fclose(fichier);
        return;
    }
    
    // 4. Lire les étudiants depuis le fichier
    // On suppose qu'il existe une fonction pour lire un étudiant depuis un fichier
    // Si elle n'existe pas, voici une implémentation simple :
    int i = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) && i < n) {
        // Parser la ligne (format: matricule TAB nom TAB prenom TAB date TAB departement TAB filiere TAB region TAB sexe)
        if (sscanf(ligne, "%s %s %s %d/%d/%d %s %s %s %c",
                   etudiants[i].matricule,
                   etudiants[i].nom,
                   etudiants[i].prenom,
                   &etudiants[i].dateNaissance.jour,
                   &etudiants[i].dateNaissance.mois,
                   &etudiants[i].dateNaissance.annee,
                   etudiants[i].departement,
                   etudiants[i].filiere,
                   etudiants[i].region,
                   &etudiants[i].sexe) >= 9) {
            i++;
        }
    }
    fclose(fichier);
    
    // 5. Trier le tableau avec tri à bulles
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comparer(&etudiants[j], &etudiants[j + 1]) > 0) {
                // Échanger les étudiants
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
    
    // 6. Afficher les étudiants triés
    printf("\n=== LISTE TRIEE PAR %s ===\n", critere);
    printf("Matricule\tNom\t\tPrenom\t\tNaissance\tDepartement\tFiliere\t\t\tRegion\tSexe\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%s\t%-12s\t%-12s\t%02d/%02d/%04d\t%-12s\t%-20s\t%-8s\t%c\n",
               etudiants[i].matricule,
               etudiants[i].nom,
               etudiants[i].prenom,
               etudiants[i].dateNaissance.jour,
               etudiants[i].dateNaissance.mois,
               etudiants[i].dateNaissance.annee,
               etudiants[i].departement,
               etudiants[i].filiere,
               etudiants[i].region,
               etudiants[i].sexe);
    }
    
    printf("\nTotal: %d etudiant(s)\n", n);
    
    // 7. Libérer la mémoire
    free(etudiants);
}

// ==================== FONCTIONS PUBLIQUES DE TRI ====================

void trierParNom(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParNom, "NOM");
}

void trierParMatricule(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParMatricule, "MATRICULE");
}

void trierParRegion(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParRegion, "REGION");
}

void trierParFiliere(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParFiliere, "FILIERE");
}