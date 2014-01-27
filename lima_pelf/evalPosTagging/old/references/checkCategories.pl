#! /usr/bin/perl -s

#r�cup�ration des categories;
open(CODE,"$ARGV[0]") || die ("impossible d ouvrir le fichier $ARGV[0]");

while ($ligne = <CODE>)
{
    chomp($ligne);
    if ($ligne=~ /<n>/)
	{
        $ligne=~ s/^.+<n>//;
		$ligne=~ s/<\/n>.*$//;

        $categs{$ligne} = 1;
        print "read categ : $ligne\n";

    }
}

# v�rification de toutes les categories
open(REF,"$ARGV[1]") || die ("impossible d ouvrir le fichier $ARGV[1]");

$i=0;
while ($ligne=<REF>)
{
    $i++;
    @infos=split(/\|/,$ligne);
	chomp($infos[2]);
	$infos[2]=~ s/^\s*//;
	if ($categs{$infos[2]}!=1)
	{
	    print "ligne $i : categorie $infos[2] is invalid !\n";
	}
}
