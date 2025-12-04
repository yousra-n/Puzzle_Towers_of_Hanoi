
# 🎮 Interface Graphique - Tours de Hanoï

## 📝 Description
Interface web interactive pour visualiser l'algorithme des Tours de Hanoï en temps réel.

Cette GUI permet de comprendre visuellement comment fonctionne l'algorithme récursif et itératif des Tours de Hanoï.

## ✨ Fonctionnalités

- ✅ **Visualisation en temps réel** : Voir les disques se déplacer entre les tours
- ✅ **Contrôle de vitesse** : Ajuster la vitesse d'animation (100ms à 2000ms)
- ✅ **Nombre de disques variable** : Tester avec 1 à 8 disques
- ✅ **Statistiques en direct** : 
  - Nombre de mouvements effectués
  - Total de mouvements requis (2ⁿ - 1)
  - Progression en pourcentage
- ✅ **Contrôles interactifs** : Démarrer, Pause, Réinitialiser
- ✅ **Interface responsive** : Fonctionne sur tous les navigateurs modernes


## 🎯 Utilisation de l'interface

1. **Choisir le nombre de disques** : Utilisez le slider "Disques" (1-8)
2. **Ajuster la vitesse** : Utilisez le slider "Vitesse" (100ms-2000ms)
3. **Lancer l'animation** : Cliquez sur "▶ Démarrer"
4. **Mettre en pause** : Cliquez sur "⏸ Pause"
5. **Réinitialiser** : Cliquez sur "↻ Réinitialiser"

## 🛠️ Technologies utilisées

- **HTML5** : Structure de la page
- **CSS3** : Design et animations
- **JavaScript (Vanilla)** : Logique de l'algorithme et interactivité
- **Aucune dépendance externe** : Fonctionne hors ligne !

## 📊 Complexité de l'algorithme

- **Complexité temporelle** : O(2ⁿ)
- **Nombre de mouvements** : 2ⁿ - 1
- **Complexité spatiale** : O(n)

### Exemple de temps d'exécution :
| Disques (n) | Mouvements | Temps approximatif |
|-------------|------------|-------------------|
| 3           | 7          | < 1 seconde       |
| 5           | 31         | < 1 seconde       |
| 10          | 1,023      | Quelques secondes |
| 20          | 1,048,575  | Plusieurs minutes |

## 📌 Note importante

 Cette interface graphique est un **complément optionnel** au projet principal.

Le projet principal est implémenté en **C++** avec :
- Version récursive
- Version itérative
- Analyse comparative des performances
- Mesures de temps d'exécution

Cette GUI sert uniquement à des fins de **visualisation pédagogique** et de **démonstration**.

## 📄 Licence

Ce projet est réalisé dans le cadre du cours "Advanced Algorithms and Complexity" 
USTHB 2025-2026.
