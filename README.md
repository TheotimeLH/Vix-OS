# Vix-OS
A home made OS that should run Vim.\
By Felix , Samuel , Florian

## Compilation
**Dépendances**
- pour la compilation : *i686-elf-gcc*, *i686-elf-g++*, *i686-elf-as*
- pour l'execution : *qemu-system-i386*

**Makefile**

*make* compile et execute le noyau, ainsi que les différents programmes utilisateurs, et créé un système de
fichier fat dans le fichier *kernel/disque* dans lequel les programmes utilisateurs sont copiés (d'où la nécessité des *sudos*)

## Programmes utilisateurs

Le shell, mini-vim, et pacman sont des programmes utilisateurs.\
Le dossier *stdlib* contient tout ce qui est nécessaire au
fonctionnement des programmes utilisateurs.\
Tous les autres dossiers concernent des fonctionnalités du noyau.