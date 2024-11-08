/**
 * @file translation.mjs
 * @module translation
 * @author Timotej Ponek, xponek00
 * translation library for project 1 in WAP course
 * VUT FIT Brno
 */

/** Constructor for BaseAminoAcid class 
 * @constructor
 */
export function BaseAminoAcid() {}

/** Prototype object representing Fenylalanin aminoacid, inherits from BaseAminoAcid 
 * @type {{}}
 */
var Fenylalanin = {name: 'Fenylalanin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Leucin aminoacid, inherits from BaseAminoAcid 
 * @type {{}}
 */
var Leucin = {name: 'Leucin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Isoleucin aminoacid, inherits from BaseAminoAcid 
 * @type {{}}
 */
var Isoleucin = {name: 'Isoleucin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Methionin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Methionin = {name: 'Methionin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Valin aminoacid, inherits from BaseAminoAcid 
 * @type {{}}
 */
var Valin = {name: 'Valin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Serin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Serin = {name: 'Serin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Prolin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Prolin = {name: 'Prolin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Threonin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Threonin = {name: 'Threonin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Alanin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Alanin = {name: 'Alanin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Tyrosin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Tyrosin = {name: 'Tyrosin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Histidin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Histidin = {name: 'Histidin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Glutamin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Glutamin = {name: 'Glutamin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Asparagin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Asparagin = {name: 'Asparagin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Lysin aminoacid, inherits from BaseAminoAcid 
 * @type {{}}
*/
var Lysin = {name: 'Lysin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Kyselina asparagová aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var KyselinaAsparagova = {name: 'Kyselina asparagová', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Kyselina glutamová, inherits from BaseAminoAcid
 * @type {{}}
 */
var KyselinaGlutamova = {name: 'Kyselina glutamová', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Cystein aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Cystein = {name: 'Cystein', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Tryptofan aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Tryptofan = {name: 'Tryptofan', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Arginin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Arginin = {name: 'Arginin', __proto__: BaseAminoAcid.prototype}
/** Prototype object representing Glycin aminoacid, inherits from BaseAminoAcid
 * @type {{}}
 */
var Glycin = {name: 'Glycin', __proto__: BaseAminoAcid.prototype}

/**
 * Class representing translation error, thrown from function translate when input is invalid
 * @class
 * @extends Error
 */
export class TranslationError extends Error {
  /**
   * @constructor
   * @param {String} message message to be displayed
   */
  constructor(message) {
    super(message);
    this.name = 'TranslationError';
  }
}

/**
 * Generator that yields objects representing aminoacids
 * @generator
 * @function translate
 * @param {String} input string with literals representing codons that are translated to aminoacids
 * @yields {{}} object representing aminoacid
 * @throws {TranslationError} invalid input, aminoacid could not be created
 */
export function* translate(input) {
  let i = 0;
  while (i < input.length) {
    // if input is too short, we get string of length less than 3 (no error raised)
    var codon = input.slice(i, i + 3).toUpperCase();
    switch(codon){
      case 'UUU':
      case 'UUC':
        yield Object.create(Fenylalanin);
        break;
      case 'UUA':
      case 'UUG':
      case 'CUU':
      case 'CUC':
      case 'CUA':
      case 'CUG':
        yield Object.create(Leucin);
        break;
      case 'AUU':
      case 'AUC':
      case 'AUA':
        yield Object.create(Isoleucin);
        break;
      case 'AUG':
        yield Object.create(Methionin);
        break;
      case 'GUU':
      case 'GUC':
      case 'GUA':
      case 'GUG':
        yield Object.create(Valin);
        break;
      case 'UCU':
      case 'UCC':
      case 'UCA':
      case 'UCG':
      case 'AGU':
      case 'AGC':
        yield Object.create(Serin);
        break;
      case 'CCU':
      case 'CCC':
      case 'CCA':
      case 'CCG':
        yield Object.create(Prolin);
        break;
      case 'ACU':
      case 'ACC':
      case 'ACA':
      case 'ACG':
        yield Object.create(Threonin);
        break;
      case 'GCU':
      case 'GCC':
      case 'GCA':
      case 'GCG':
        yield Object.create(Alanin);
        break;
      case 'UAU':
      case 'UAC':
        yield Object.create(Tyrosin);
        break;
      case 'CAU':
      case 'CAC':
        yield Object.create(Histidin);
        break;
      case 'CAA':
      case 'CAG':
        yield Object.create(Glutamin);
        break;
      case 'AAU':
      case 'AAC':
        yield Object.create(Asparagin);
        break;
      case 'AAA':
      case 'AAG':
        yield Object.create(Lysin);
        break;
      case 'GAU':
      case 'GAC':
        yield Object.create(KyselinaAsparagova);
        break;
      case 'GAA':
      case 'GAG':
        yield Object.create(KyselinaGlutamova);
        break;
      case 'UGU':
      case 'UGC':
        yield Object.create(Cystein);
        break;
      case 'UGG':
        yield Object.create(Tryptofan);
        break;
      case 'CGU':
      case 'CGC':
      case 'CGA':
      case 'CGG':
      case 'AGA':
      case 'AGG':
        yield Object.create(Arginin);
        break;
      case 'GGU':
      case 'GGC':
      case 'GGA':
      case 'GGG':
        yield Object.create(Glycin);
        break;
      case 'UGA':
      case 'UAA':
      case 'UAG':
        // stop codon encountered, exit
        return;
      default:
        // bad input
        throw new TranslationError(`Invalid codon: ${codon}`);
    }
    i += 3;
  }
}