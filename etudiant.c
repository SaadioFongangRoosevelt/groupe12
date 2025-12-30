#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Etudiant.h"



// comparaison pour le tri par nom
int comparerParNom(const Etudiant *a, const Etudiant *b) {
    int cmpNom = strcasecmp(a->nom, b->nom);
    if (cmpNom != 0) return cmpNom;
    return strcasecmp(a->prenom, b->prenom);
}

// comparaison pour le tri par matricule
int comparerParMatricule(const Etudiant *a, const Etudiant *b) {
    return strcmp(a->matricule, b->matricule);
}

// comparaison pour le tri par région
int comparerParRegion(const Etudiant *a, const Etudiant *b) {
    int cmpRegion = strcasecmp(a->region, b->region);
    if (cmpRegion != 0) return cmpRegion;
    return comparerParNom(a, b);
}

// comparaison pour le tri par filière
int comparerParFiliere(const Etudiant *a, const Etudiant *b) {
    int cmpFiliere = strcasecmp(a->filiere, b->filiere);
    if (cmpFiliere != 0) return cmpFiliere;
    return comparerParNom(a, b);
}

// Fonction pour lire, trier et afficher les étudiants
static void trierEtAfficher(const char *nomFichier, 
                          int (*comparer)(const Etudiant*, const Etudiant*), 
                          const char *critere) {
    int n = nombreEtudiant(nomFichier);
    if (n == 0) {
        printf("Aucun etudiant a trier.\n");
        return;
    }
    
    
    FILE *fichier = fopen(nomFichier, "r");// Ouvrir le fichier
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }
    
   
    Etudiant *etudiants = malloc(n * sizeof(Etudiant)); // Allouer un tableau pour stocker les étudiants
    if (etudiants == NULL) {
        printf("Erreur d'allocation memoire.\n");
        fclose(fichier);
        return;
    }
    
    // Lire les étudiants depuis le fichier
    int i = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) && i < n) {
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
    
    //Trier avec le tri à bulles
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
    
    // Afficher les étudiants triés
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
    
    //  Libérer la mémoire
    free(etudiants);
}


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