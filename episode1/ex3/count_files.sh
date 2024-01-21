#!/bin/bash

# Compter les fichiers reguliers
nb_files=$(find . -maxdepth 1 -type f | wc -l)

# Compter les dossiers
nb_dirs=$(find . -maxdepth 1 -type d | wc -l)
nb_dirs=$((nb_dirs - 1)) # Soustraire le repertoire courant '.'

# Afficher les resultats
echo "Nombre de fichiers reguliers: $nb_files"
echo "Nombre de dossiers: $nb_dirs"