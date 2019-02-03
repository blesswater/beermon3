$(function($) {

    $(".knob").knob({
        change : function (value) {
            //console.log("change : " + value);
        },
        release : function (value) {
            //console.log(this.$.attr('value'));
            console.log("release : " + value);
        },
        cancel : function () {
            console.log("cancel : ", this);
        },
        /*format : function (value) {
            return value + '%';
        },*/
        draw : function () {

            // "tron" case
            if(this.$.data('skin') == 'tron') {

                this.cursorExt = 0.3;

                var a = this.arc(this.cv)  // Arc
                    , pa                   // Previous arc
                    , r = 1;

                this.g.lineWidth = this.lineWidth;

                if (this.o.displayPrevious) {
                    pa = this.arc(this.v);
                    this.g.beginPath();
                    this.g.strokeStyle = this.pColor;
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, pa.s, pa.e, pa.d);
                    this.g.stroke();
                }

                this.g.beginPath();
                this.g.strokeStyle = r ? this.o.fgColor : this.fgColor ;
                this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, a.s, a.e, a.d);
                this.g.stroke();

                this.g.lineWidth = 2;
                this.g.beginPath();
                this.g.strokeStyle = this.o.fgColor;
                this.g.arc( this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false);
                this.g.stroke();

                return false;
            }
        }
    });

    // Example of infinite knob, iPod click wheel
    var v, up=0,down=0,i=0
        ,$idir = $("div.idir")
        ,$ival = $("div.ival")
        ,incr = function() { i++; $idir.show().html("+").fadeOut(); $ival.html(i); }
        ,decr = function() { i--; $idir.show().html("-").fadeOut(); $ival.html(i); };
    $("input.infinite").knob(
                        {
                        min : 0
                        , max : 20
                        , stopper : false
                        , change : function () {
                                        if(v > this.cv){
                                            if(up){
                                                decr();
                                                up=0;
                                            }else{up=1;down=0;}
                                        } else {
                                            if(v < this.cv){
                                                if(down){
                                                    incr();
                                                    down=0;
                                                }else{down=1;up=0;}
                                            }
                                        }
                                        v = this.cv;
                                    }
                        });
});


/* Control knob state machine */
/*
** States:
**     0: SwitchedOut
**     1: Monitor
**     2: Ready
**     3: Adjust
**
** Events:
**    0: switchOut
**    1: switchInMon
**    2: switchInCntl
**    3: Monitor
**    4: Control
**    5: Release
**    6: Submit
**    7: Clear
**    8: dataStat
*/
var controlKnobSM = [
    /* Switched Out State */
    [
         /* Switch Out Event */
         function( knobState ) { console.log( "State: 0, Event: 0 ");
             _clearKnob();
             return 0
         },
         /* SwitchInMon */
         function(knobState) {
             console.log( "State: 0, Event: 1 ");
             td = document.getElementById( "setpointControlTd" );
             knob = createKnob(knobState);
             knob.setAttribute( "data-readonly", "readonly" );
             td.appendChild( knob );
             $(".knob").knob();
             return 1
         },
         /* SwitchInCntl */
         function(knobState) { console.log( "State: 0, Event: 2 ");
             console.log( "State: 1, Event: 1 ");
             td = document.getElementById( "setpointControlTd" );
             knob = createKnob(knobState);
             td.appendChild( knob );
             $(".knob").knob();
             return 2
         },
         function(knobState) { console.log( "State: 0, Event: 3 ");
             return 0
         },
         function(knobState) { console.log( "State: 0, Event: 4 ");
                      return 0
                    },
         function(knobState) { console.log( "State: 0, Event: 5 ");
                      return 0
                    },
         function(knobState) { console.log( "State: 0, Event: 6 ");
                      return 0
                    },
         function(knobState) { console.log( "State: 0, Event: 7 ");
                      return 0
                    },
         function(knobState) { console.log( "State: 0, Event: 8 ");
                      return 0
                    },
    ],
    /* Monitor State */
    [
        /* Switch Out Event */
        function(knobState) { console.log( "State: 1, Event: 0 ");
             _clearKnob();
             return 0
        },
        function(knobState) { console.log( "State: 1, Event: 1 ");
            return 1
        },
        function(knobState) { console.log( "State: 1, Event: 2 ");
            return 1
        },
        function(knobState) { console.log( "State: 1, Event: 3 ");
            return 1
        },
        /* Control */
        function(knobState) {
            console.log( "State: 1, Event: 4 ");
            _clearKnob();
            td = document.getElementById( "setpointControlTd" );
            knob = createKnob(knobState);
            td.appendChild( knob );
            $(".knob").knob();
            return 2
        },
        function(knobState) { console.log( "State: 1, Event: 5 ");
           return 1
        },
        function(knobState) { console.log( "State: 1, Event: 6 ");
            return 1
        },
        function(knobState) { console.log( "State: 1, Event: 7 ");
            return 1
        },
        function(knobState) { console.log( "State: 1, Event: 8 ");
            return 1
        },
    ],
    /* Control State */
    [
        /* Switch Out Event */
        function(knobState) { console.log( "State: 2, Event: 0 ");
            _clearKnob();
            return 0
        },

        function(knobState) { console.log( "State: 2, Event: 1 ");
            return 2
        },
         function(knobState) { console.log( "State: 2, Event: 2 ");
                      return 2
                    },
        /* Monitor Event */
        function(knobState) {
            console.log( "State: 2, Event: 3 ");
            _clearKnob();
            td = document.getElementById( "setpointControlTd" );
            knob = createKnob(knobState);
            knob.setAttribute( "data-readonly", "readonly" );
            knob.value = "65"
            td.appendChild( knob );
            $(".knob").knob();
            return 1
        },
        /* Control */
        function(knobState) { console.log( "State: 2, Event: 4 ");
            return 2
        },
         function(knobState) { console.log( "State: 2, Event: 5 ");
                      return 2
                    },
         function(knobState) { console.log( "State: 2, Event: 6 ");
                      return 2
                    },
         function(knobState) { console.log( "State: 2, Event: 7 ");
                      return 2
                    },
         function(knobState) { console.log( "State: 2, Event: 8 ");
                      return 2
                    },
    ],
    /* Adjust State */
    [
        /* Switch Out Event */
        function(knobState) {
            console.log( "State: 3, Event: 0 ");
           _clearKnob();
           return 3
        },
        function(knobState) { console.log( "State: 3, Event: 1 ");
            return 3
        },
        function(knobState) { console.log( "State: 3, Event: 2 ");
            return 3
        },
        /* Monitor Event */
        function(knobState) { console.log( "State: 3, Event: 3 ");
            _clearKnob();
            td = document.getElementById( "setpointControlTd" );
            knob = createKnob(knobState);
            knob.setAttribute( "data-readonly", "readonly" );
            td.appendChild( knob );
            $(".knob").knob();
            return 1
        },
         function(knobState) { console.log( "State: 3, Event: 4 ");
                      return 3
                    },
         function(knobState) { console.log( "State: 3, Event: 5 ");
                      return 3
                    },
         function(knobState) { console.log( "State: 3, Event: 6 ");
                      return 3
                    },
         function(knobState) { console.log( "State: 3, Event: 7 ");
                      return 3
                    },
         function(knobState) { console.log( "State: 3, Event: 8 ");
                      return 3
                    },
    ],
]

function createKnob(knobState) {
    knob = document.createElement( "input" );
    knob.setAttribute( "class", "knob controlEnabled");
    knob.setAttribute( "data-min", knobState.minRange );
    knob.setAttribute( "data-max", knobState.maxRange );
    knob.setAttribute( "data-step", "0.5" );
    knob.setAttribute( "data-angleOffset", "-125");
    knob.setAttribute( "data-angleArc", "250" );
    knob.setAttribute( "data-fgColor", "#66EE66" );
    knob.setAttribute( "data-rotation", "clockwise" );
    knob.setAttribute( "value", knobState.value );

    return knob;
}

function _clearKnob() {
    td = document.getElementById( "setpointControlTd" );
    while( td.firstChild ) {
        td.removeChild( td.firstChild );
    }
}

function processControlKnobSM( knobState, knobEvent ) {
    knobState.state = controlKnobSM[knobState.state][knobEvent](knobState);
}

function initControlKnob( knobState ) {
     knobState.state = 0;
}

function processKnobEvent( knobEvent ) {
    if( userSelections.setpointControlKnob.hasOwnProperty( userSelections.selDatasetValue ) ) {
        processControlKnobSM( userSelections.setpointControlKnob[userSelections.selDatasetValue], knobEvent )
    }
}