        READMEFIRST v1.00 last update July 20, 2008 by B. Hahne

This READMEFIRST file accompanies the cgspeed.com "BVH conversion"
release of the Carnegie-Mellon University (CMU) Graphics Lab Motion
Capture Database.  See "Where to find stuff" at the bottom of this
file for where to get the BVH conversion and/or the original CMU
dataset.

The original CMU motion capture database isn't in BVH format - it's
in ASF/AMC format.  This BVH conversion release was created by Bruce
Hahne, a hobbyist animator, in the interest of making the data more
available and easily usable by other animators.  I presently (2008)
maintain the web site www.cgspeed.com, where this BVH conversion
release will be available or linked.

The emphasis on this release is to produce BVH files that can rapidly
be used in MotionBuilder for motion retargetting.  The files are not
yet particularly Poser-friendly or DazStudio-friendly, due to
incorrect assumptions that those programs have to make about the
underlying joint rotation setup.



ADVANTAGES OF THIS RELEASE OVER THE ORIGINAL CMU DATA:
- This release has the motions in BVH format. :-)

- T-poses: Every BVH file has a T-pose added as its new first frame.
  The T-poses face the positive Z axis and are therefore MotionBuilder
  compatible.

- Joint renaming: As many joints as possible have been renamed to be
  compatible with MotionBuilder's joint naming conventions.  This renaming
  makes it much easier (faster) to use the BVH files in MotionBuilder.

- Index files: The release includes consolidated indexes that list
  the motion filenames and their descriptions.  Both spreadsheet and 
  word processor friendly index files are available.

- MotionBuilder-friendly: the joint renaming and the addition of the
  T-pose make it possible to use these motions for animation
  retargetting within MotionBuilder in seconds rather than minutes.


CONVERSION PROCESS:

- I started with the full set of AMC and ASF files, available from CMU
  as a single .zip file
- I ran the AMC/ASF files through amc2bvh in batch mode to produce BVH files.
- I analyzed several of the BVH files in MotionBuilder to determine how
  to set joint angles to set a T-pose.
- I then ran the BVH files through a Python script that I wrote to do
  joint naming conversion and T-pose generation.  I haven't yet (July 2008) 
  released this script.
- With the exception of the new frame 1 (the T-pose), there are no
  modifications to the motion data itself.  If the original CMU data
  is noisy, the conversion will be noisy.  If the original CMU data
  has the actor's head moving at an impossible angle, the conversion
  will do the same.


CONVERSION NOTES:

Shoulders: The CMU dataset's skeleton is somewhat annoying around the
shoulders because it doesn't use a clavicle joint, only a shoulder
joint.  Also, the rotation points of the CMU skeleton shoulders are
significantly wider than the hips.  The result was that when I did
some retargetting tests in MotionBuilder, I often found that the
geometry of the target character's arms would punch through the hips.
To somewhat mitigate this problem, I set the T-pose of this converted
CMU dataset such that the arms angle slightly downwards, at 5 degrees
down from the horizontal.  This adjustment helps to reduce the "arms
through the hips" problem.


Fingers:  CMU's main page says this about the dataset:

  The "finger" and "thumb" joints are added to the skeleton for editing
  convenience - we do not actually capture these joints' motions and any
  such data should be ignored.

Therefore in the BVH joint naming conversion, I've maintained the
mapping "lfingers" to "LFingers" and "lthumb" to "LThumb", since these
are names that MotionBuilder's character joint naming convention does
NOT recognize.  We don't want MotionBuilder to try to these joints,
since they have no data on them.


Playback speed: The CMU dataset was sampled at 120fps, however this
information apparently isn't saved in the CMU-distributed AMC/ASF
files, and the freeware utility amc2bvh simply assumes a default value
of 30fps (Frame Time = .033333) when it writes out BVH files.  This
BVH conversion release fixes the problem by rewriting Frame Time to
.0083333 in each BVH file, which equates to 120fps.


INDEX/INFORMATION FILES: 

Each .zip file in this BVH conversion release should include a copy of
this READMEFIRST.txt file, plus four variations of the same motion
index information:
- cmu-mocap-index-spreadsheet.ods: Open Document / OpenOffice
  spreadsheet format
- cmu-mocap-index-spreadsheet.xls: Microsoft Excel format
- cmu-mocap-index-txt.txt: A simple text file with the index
  information and no commentary.
- cmu-mocap-index-text.rtf: Rich Text Format index information with
  some commentary.


USAGE RIGHTS:

CMU places no restrictions on the use of the original dataset, and I
(Bruce) place no additional restrictions on the use of this particular
BVH conversion.

Here's the relevant paragraph from mocap.cs.cmu.edu:

  Use this data!  This data is free for use in research and commercial
  projects worldwide.  If you publish results obtained using this data,
  we would appreciate it if you would send the citation to your
  published paper to jkh+mocap@cs.cmu.edu, and also would add this text
  to your acknowledgments section: "The data used in this project was
  obtained from mocap.cs.cmu.edu.  The database was created with funding
  from NSF EIA-0196217."


JOINT RENAMING TEMPLATE

Here's the joint renaming template that my unreleased Python script 
uses, with a variety of comments that come from my analysis of the skeleton.
The left column is the original joint name used by the CMU dataset,
and the right column is the joint name that my script outputs.  Most
of the time this right-column name is one that MotionBuilder
recognizes, however in some cases we have to intentionally convert to
a name that MotionBuilder won't recognize, because we don't want
MotionBuilder to use the joint when we characterize.

---------------------------------------------------------------------
# Some facts about the CMU skeleton:
# lhipjoint and rhipjoint are in the same location as hip, and have no
#   keyframes.
#
# lowerback is in the same location as hip, and has keyframes. MotionBuilder
# docs say that Spine must NOT be in same location as hip, so we have to
# throw out "lowerback" and not call it Spine.
#
# thorax, lowerneck, lclavicle, and rclavicle are all in the same location.
# thorax and lowerneck are keyed.  lclavicle and rclavicle are not.
#
# Although we have two neck joints, the CMU "lowerneck" is really in
# the middle of the spine -- it sure doesn't look anywhere close to
# the human neck to me.  However when I tried calling it Spine2,
# MB gave a warning: "Hierarchy warning: LeftShoulder is not the direct
# descendant of Spine2, Spine1 was found".
#
# Per CMU web page: 'The "finger" and "thumb" joints are added to the
# skeleton for editing convenience - we do not actually capture these
# joints' motions and any such data should be ignored.'

TEMPLATE: cmu
hip                                     Hips

# Not sure what to do with lhipjoint.  It has no keyframes and is
# just a connector to attach the leg, so probably OK to rename it
# in a way that MB won't use it.
 lhipjoint                              LHipJoint  # Not used by MB
  lfemur                                LeftUpLeg
   ltibia                               LeftLeg
    lfoot                               LeftFoot
     ltoes                              LeftToeBase
 rhipjoint                              RHipJoint
   rfemur                               RightUpLeg
    rtibia                              RightLeg
     rfoot                              RightFoot
      rtoes                             RightToeBase

# We can't translate "lowerback" to Spine because lowerback is in
# the same location as Hips.  The MB docs say that you shouldn't do
# that.
 lowerback                              LowerBack  # child of hips
  upperback                             Spine
   thorax                               Spine1

# "Lower" and "upper" neck?  Good grief.
    lowerneck                           Neck
     upperneck                          Neck1
      head                              Head
    lclavicle                           LeftShoulder    # child of
    thorax
     lhumerus                           LeftArm
      lradius                           LeftForeArm
       lwrist                           LeftHand
        lhand                           LeftFingerBase
# LFingers and LThumb aren't names used by MB.  CMU notes that there's
# no data on these joints, so we want to ignore them.
         lfingers                       LFingers
        lthumb                          LThumb  # child of lwrist
    rclavicle                           RightShoulder # child of thorax
     rhumerus                           RightArm
      rradius                           RightForeArm
       rwrist                           RightHand
        rhand                           RightFingerBase
         rfingers                       RFingers
        rthumb                          RThumb  # child of rwrist

# END OF CMU JOINT RENAMING TEMPLATE with comments
-----------------------------------------------------------------


FUTURE PROJECT IDEAS:
- Write a tutorial that shows MotionBuilder workflow for how to use a
  BVH file.

- Make a Poser-friendly / DAZ-friendly release of this dataset.  Doing
  so will almost certainly require first retargeting the data, to
  correct joint rotation problems.

- Make an .fbx release of this dataset, such that each .fbx file can
  be imported directly into the MotionBuilder Story window as a
  "motion clip".

- Clean/smooth the data?

- Any possibility of making this data SecondLife-friendly, or
  importing it into MovieStorm?


CHALLENGE TO PROGRAMMERS:

The hobbyist and animation prosumer world needs a lower-cost
alternative to Autodesk Motionbuilder, which at $3500 MSRP is out of
reach of the hobbyist animator.  Today for $50 I can buy a wide
variety of consumer software packages that let me import, edit,
refine, and transition a series of home video clips, but if I want to
import, edit, refine, and transition a series of keyframes on the
joints of an animated character, I have to pay $3500.  The first
software company or individual to produce a skeleton animation editor
which is BVH-friendly, DAZ-friendly, Poser-Friendly, and
SecondLife-friendly, with clean animation imports, exports, and
cross-rig animation retargeting for under $250, will make a mint.



CONTACT INFO AND WHERE TO FIND STUFF:
  This BVH conversion release: www.cgspeed.com
  Original CMU catabase (no BVH): mocap.cs.cmu.edu
  AMC2BVH freeware utility: http://vipbase.net/amc2bvh/
  MotionBuilder: www.autodesk.com/motionbuilder

To contact the creator of this BVH conversion release: hahne@io.com

If you like this BVH conversion release, feel free to drop me email
and let me know what you're doing with it.  If it turns out that
nobody needs a BVH dataset like this, then there's probably no point
in me doing public releases of other improvements.
