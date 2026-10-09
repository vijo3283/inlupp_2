Flöde för varje arbetspass
1. Innan ni börjar jobba
Se till att inga ocommittade ändringar ligger kvar:
git status

Hämta sedan senaste versionen:
git pull --no-rebase origin main



2. När du är klar med en ändring
Lägg till och committa de filer du ändrat:
git add filnamn
git commit -m "Kort beskrivning av ändringen"


Eller lägg till alla ändrade filer om du har kollat att de hör till ändringen:
git add .
git commit -m "Kort beskrivning av ändringen"


3. Innan du pushar
Hämta igen, eftersom din partner kan ha hunnit pusha medan du arbetade:
git pull --no-rebase origin main

Om Git hittar konflikter öppnar ni dem i Merge Editor i VS Code. Välj och kombinera ändringarna, spara filerna och slutför sedan merge-committen. Därefter:
git push origin main



Om pushen nekas
Det betyder oftast att den andra personen hann pusha först. Kör då:
git pull --no-rebase origin main

Lös eventuella konflikter i Merge Editor och försök sedan igen:
git push origin main


Viktigt: använd inte git push --force för att komma runt ett nekande push. Det kan skriva över den andras arbete.
