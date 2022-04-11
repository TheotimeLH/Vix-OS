___methodes de Fat_entry pour la lecture de donnée :___

uint32_t read_data(uint8_t *buffer,uint32_t cluster_count,Fat_infos* infos,Fat_system* intf);

		uniquement pour les fichiers
		renvoit la taille lue en octet
		si elle est inférieure à cluster_count*infos.byte_per_cluster (la taille du buffer),
		c'est que la fin du fichier a été atteinte
		utiliser init_offset pour repartir du début
		attention, le buffer doit être de taille cluster_count*infos.byte_per_cluster

uint32_t read_entries(Fat_entry* buffer,uint32_t size,Fat_infos* infos,Fat_system* intf);

		uniquement pour les répertoires
		renvoit le nombre d'entrées lues, s'il est inférieur à size,
		c'est que toutes les entrées du répertoire ont été lues
		(size est la taille du buffer)
		utiliser init_offset pour repartir du début
		taille de buffer conseillée :
			- pour la racine en fat12/16 : infos.sector_size/32
			- sinon, infos.byte_per_cluster/32

exemple d'utilisation dans main

___pour creer un systeme de fichier :____

dd if=/dev/zero of=mon_fichier count=nb_secteurs //un secteur=512 octets
mkfs.fat -n nom_volume mon_fichier
sudo mount mon_fichier /mnt/dossier_vide
//travail ds /mnt/dossier_vide
sudo umount mon_fichier
