package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;

import java.time.LocalDate;

/**
 * Prvá úloha do predmetu IJA
 * @file StoreGoodsItem.java
 * @author Timotej Ponek xponek00
 */

public class StoreGoodsItem implements GoodsItem {

    private String name;
    private LocalDate date;
    // just a reference to Box where item belongs,
    // should be smth like pointer in C if I understand it
    private Goods ref;


    // variable cannot be null
    public StoreGoodsItem(Goods variable, LocalDate date) {
        name = variable.getName();
        this.date = date;
        ref = variable;
    }

    @Override
    public Goods goods() {
        return ref;
    }

    @Override
    public boolean sell() {
        return ref != null ? ref.remove(this) : false;
    }
}
