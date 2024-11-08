'use strict'
import { test } from 'uvu';
import * as assert from 'uvu/assert';


import {translate, BaseAminoAcid, TranslationError} from './translation.mjs';

test('Traslation of all known codons', () => {
    // aminoacid containing all possible codons
    const allAminoAcid = 'UUUUUCUUAUUGCUUCUCCUACUGAUUAUCAUAAUGGUU\
GUCGUAGUGUCUUCCUCAUCGAGUAGCCCUCCCCCACCGACUACCACAACGGCUGCCGCAGCGUAUUACCAU\
CACCAACAGAAUAACAAAAAGGAUGACGAAGAGUGUUGCUGGCGUCGCCGACGGAGAAGGGGUGGCGGAGGGUAA'
    const iterator = translate(allAminoAcid);
    let aminoAcid = iterator.next();

    assert.is(aminoAcid.value.name, 'Fenylalanin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Fenylalanin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Leucin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Isoleucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Isoleucin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Isoleucin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Methionin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Valin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Valin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Valin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Valin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Serin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Prolin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Prolin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Prolin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Prolin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Threonin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Threonin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Threonin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Threonin')
    
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Alanin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Alanin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Alanin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Alanin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Tyrosin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Tyrosin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Histidin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Histidin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glutamin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glutamin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Asparagin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Asparagin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Lysin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Lysin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Kyselina asparagová')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Kyselina asparagová')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Kyselina glutamová')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Kyselina glutamová')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Cystein')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Cystein')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Tryptofan')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Arginin')

    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glycin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glycin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glycin')
    aminoAcid = iterator.next();
    assert.is(aminoAcid.value.name, 'Glycin')

    // no more codons to process
    aminoAcid = iterator.next();
    assert.ok(aminoAcid.done)
});


//TODO test for stopcodons
test('Test for stopcodon', () => {
  const iterator = translate('UUUAAUUGAGG')

  let aminoAcid = iterator.next();
  assert.not.ok(aminoAcid.done)
  aminoAcid = iterator.next();
  assert.not.ok(aminoAcid.done)

  // Stop codon encountered, exit generator
  aminoAcid = iterator.next();
  assert.ok(aminoAcid.done)

  // Subsequent call to iterator.next() won't change it's exit state
  aminoAcid = iterator.next();
  assert.ok(aminoAcid.done)
})

test('Invalid input', () => {
  const iterator = translate('XYZCGU')

  // Invalid input, throws TranslationError
  assert.throws(() => iterator.next(), err => err instanceof TranslationError)

  // Translation stops after error thrown
  let aminoAcid = iterator.next();
  assert.ok(aminoAcid.done)

  // Another examples of invalid input
  const iterator2 = translate('22X')
  assert.throws(() => iterator2.next(), err => err instanceof TranslationError)

  const iterator3 = translate('AA')
  assert.throws(() => iterator3.next(), err => err instanceof TranslationError)

})

//TODO test for is instance...
test('All aminoacids are instance of BaseAminoAcid', () => {
  const iterator = translate('GAUCAAGGA')

  let aminoAcid1 = iterator.next().value
  let aminoAcid2 = iterator.next().value
  let aminoAcid3 = iterator.next().value

  assert.instance(aminoAcid1, BaseAminoAcid)
  assert.instance(aminoAcid2, BaseAminoAcid)
  assert.instance(aminoAcid3, BaseAminoAcid)
})

test('Two same aminoacids are different objects, sharing the same prototype', () => {
  const iterator = translate('CGUCGC')

  let aminoAcid1 = iterator.next().value
  let aminoAcid2 = iterator.next().value

  // Distinct objects
  assert.is.not(aminoAcid1, aminoAcid2)

  // Same aminoacids (sharing the same prototype)
  assert.is(aminoAcid1.__proto__, aminoAcid2.__proto__)
  assert.is(aminoAcid1.name, aminoAcid2.name)
})

test('Two distinct aminoacids are different objects, not sharing the same prototype', () => {
  const iterator = translate('CGUGGU')

  let aminoAcid1 = iterator.next().value
  let aminoAcid2 = iterator.next().value

  // Distinct objects
  assert.is.not(aminoAcid1, aminoAcid2)

  // Distinct aminoacids (different prototypes)
  assert.is.not(aminoAcid1.__proto__, aminoAcid2.__proto__)
  assert.is.not(aminoAcid1.name, aminoAcid2.name)
})

test('Name of aminoacid is property of prototype, not property of an actual object returned by translate generator', () => {
  const iterator = translate('CCU')

  let aminoAcid = iterator.next().value

  // aminoacid does not directly have name property
  assert.not.ok(aminoAcid.hasOwnProperty('name'))
  // but it is a property of its prototype
  assert.ok(aminoAcid.__proto__.hasOwnProperty('name'))
})

test('Iterators are independent', () => {
  // Leucin, Isoleucin, Alanin
  const iterator1 = translate('CUCAUAGCGUGA')
  const iterator2 = translate('CUCAUAGCGUGA')

  let it1_val = iterator1.next()
  assert.is(it1_val.value.name, 'Leucin')
  it1_val =  iterator1.next()
  assert.is(it1_val.value.name, 'Isoleucin')

  let it2_val = iterator2.next()
  assert.is(it2_val.value.name, 'Leucin')

  // Diferent objects and different aminoacids
  assert.is.not(it1_val.value, it2_val.value)
  assert.is.not(it1_val.value.name, it2_val.value.name)

  it2_val = iterator2.next()
  assert.is(it2_val.value.name, 'Isoleucin')

  // Diferent objects, same aminoacids
  assert.is.not(it1_val.value, it2_val.value)
  assert.is(it1_val.value.name, it2_val.value.name)

  // iterators are distinct objects
  assert.is.not(iterator1, iterator2)
  // but are of same type
  assert.is(iterator1.__proto__, iterator2.__proto__)
})


/* test('adds 1 + 2 to is 3', () => {
  expect(sum(1, 2)).toBe(3);
}); */

test.run()