
# Skriva tester

# Gå igenom kraven i uppgiftsbeskrivningen

- Kontrollera att alla obligatoriska menyval finns och använder rätt action code: A, L, D, E, S, P, C, R, +, -, =, O och Q.
- Kontrollera att varje menyval läser indata i rätt ordning, hanterar bekräftelser enligt kraven och accepterar både stora och små bokstäver för bokstavsval.
- Testa hela användarflödet med `webstoretest.txt` och åtgärda eventuella fel eller avvikelser i utskrifter och beteende.
- Kontrollera felaktig indata och gränsfall: ogiltiga tal, hyll-id i formatet A-Z följt av två siffror, okända varor och kundvagnar, dubbletter samt kvantiteter som är noll eller negativa.
- Säkerställ att lagersaldot och hyllornas innehåll hålls synkroniserade och att varor i kundvagnar reserveras så att alla godkända beställningar går att checka ut.
- Säkerställ att checkout drar av rätt antal från rätt hyllor och tar bort kundvagnen, och att borttagning av kundvagn frigör dess reservationer.
- Gå igenom testtäckningen för backend: testa både lyckade operationer och fel-/gränsfall för alla funktioner utanför användargränssnittet.
- Kontrollera att programmet är uppdelat i generiska datastrukturer, generella verktyg, affärslogik och användargränssnitt.
- Slutför Doxygen-dokumentation för generella verktyg och affärslogik.
- Kör bygget, backend-testerna och det skriptade användarflödet under Valgrind; åtgärda minnesläckor och andra minnesfel.

# Innan inlämning

- Gå igenom punkterna ovan och kontrollera att kvarvarande TODO-punkter är lösta eller dokumenterade.
- Skicka gruppmedlemmarnas namn och användarnamn samt GitHub-länk till ioopm@it.uu.se.
- Gör och pusha den slutliga inlämningscommiten och tagga den `assignment2_done`.

# Valfria delar

- Överväg att implementera undo (upp till 16 åtgärder).
- Överväg att spara data på fil mellan programkörningar.

