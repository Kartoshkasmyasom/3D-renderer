# Tests

The tests are built together with the main project when `BUILD_TESTING` is
enabled, which is the default after `include(CTest)`.

From the repository root:

```powershell
.\build.ps1
ctest --test-dir build-vcpkg -C Debug --output-on-failure
```

The current test executable is `renderer_tests`. It covers console input
parsing and command validators without opening an OpenGL window.

Range tests cover RGB `[0, 255]`, movement `[-1000, 1000]`, rotation
`[0, 360]`, and scale `[0.1, 10]`. Each channel/axis is checked independently
with the other two components at the midpoint. Cases include the midpoint,
both inclusive endpoints, each endpoint plus/minus 1, the nearest representable
floats inside/outside each endpoint, distant out-of-range values, and the lowest
and highest finite floats. Scale also explicitly rejects zero. Accepted values
are checked for exact preservation in every request component.

Movement tests additionally pass input through the parser, validator, and move
command for all eight octants, both directions of each axis, all four sign
combinations in each coordinate plane, and zero displacement. Each case checks
all three resulting coordinates from the origin, a nonzero position, and a
position that crosses the relevant coordinate planes after moving. Zero
components must leave their coordinates unchanged. These checks supplement
the range tests and the existing accumulated-displacement test.

Scaling tests pass input through the parser, validator, and scale command, then
measure a synthetic 2 x 4 x 6 box after transforming its eight corners with the
object's model matrix. They cover uniform enlargement/reduction, enlargement
and reduction on each axis independently, nonuniform scaling on XY/XZ/YZ and
all three axes, and identity scaling from the initial unit scale. Each case runs
at the origin and at a translated position, checking all dimensions and that
the object's position is unchanged. No GPU upload or visual rendering is used.

Rotation scenario tests select an object among two scene objects, parse and
validate angles, execute rotation, and compare transformed basis directions
with explicit expected directions. They cover positive rotations on
each axis, `0 45 0`, XY/XZ/YZ/XYZ rotations, and zero angles from
both the initial and an already rotated orientation. They also check that
position and the unselected object's transform remain unchanged.

Negative input angles are invalid under the `[0, 360]` contract. Tests check
rejection on each axis separately and when mixed with positive angles.

Rotation inputs are added to the current Euler angles on each axis. Tests
verify successive additions, preservation of an existing orientation for zero
input, a 45 + 45 degree turn, and accumulated angles exceeding 360 degrees.
The `[0, 360]` limit applies to each input increment, not the accumulated angle.

Keyboard dispatch and visual rendering are not exercised by these headless
tests. To check them manually, import an asymmetric model, enter edit mode
with E, select it with Left/Right, press R, and enter the tested angles in the
console. Compare the orientation before/after, including zero angles after a
nonzero rotation, and verify that other scene objects stay unchanged. Negative
angles must trigger validation errors instead of rotating the object.
