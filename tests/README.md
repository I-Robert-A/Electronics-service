## Scop
Acest folder conține fișierele de test utilizate pentru validarea funcționalităților aplicației FixItNow.
Testele acoperă atât scenarii valide de utilizare, cât și cazuri de eroare și situații limită, conform
cerințelor din enunțul proiectului.

Aplicația nu se oprește la întâlnirea unei erori, ci continuă procesarea liniilor valide și raportează
toate problemele detectate.

---

## Structura folderului

- angajati.csv – date despre angajați (valizi și invalizi)
- marci.csv – tipuri, mărci și modele reparabile
- cereri.csv – cereri de reparație (valide și invalide)
- README.md – descrierea testelor

---

## Teste pentru angajați (`angajati.csv`)

Fișierul conține:
- tehnicieni, recepționeri și supervizori cu date valide
- angajați cu CNP invalid (lungime greșită, cifră de control greșită)
- angajați cu vârsta sub 16 ani
- nume/prenume prea scurte sau prea lungi
- angajați din București și din alte orașe (pentru testarea primei de transport)

Scop:
- validarea citirii corecte a angajaților
- verificarea calculului salariului
- verificarea mesajelor de eroare pentru linii invalide

---

## Teste pentru electrocasnice (`electrocasnice.csv`)

Fișierul conține:
- tipuri valide de electrocasnice (TV, frigider, mașină de spălat)
- mărci și modele multiple separate prin `;`
- mărci fără modele (test adăugare marcă)
- modele duplicate
- linii incomplete sau cu câmpuri lipsă

Scop:
- verificarea adăugării și ștergerii de mărci și modele
- validarea citirii corecte din fișier
- tratarea erorilor fără oprirea programului

---

## Teste pentru cereri de reparație (`cereri.csv`)

Fișierul conține:
- cereri valide pentru electrocasnice reparabile
- cereri invalide (model necunoscut, marcă necunoscută)
- cereri cu valori limită (preț 0, complexitate 0)
- cereri cu date invalide (timestamp invalid, câmpuri lipsă)

Scop:
- verificarea filtrării cererilor invalide
- stocarea cererilor invalide separat
- calculul duratei și prețului reparației
