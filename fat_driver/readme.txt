___pour creer un systeme de fichier :____

dd if=/dev/zero of=mon_fichier count=nb_secteurs //un secteur=512 octets
mkfs.fat -n nom_volume mon_fichier
sudo mount mon_fichier /mnt/dossier_vide
//travail ds /mnt/dossier_vide
sudo umount mon_fichier
