# Example 3 — Module design

# Module: resize
Satisfies: REQ-F-002, REQ-N-001, REQ-N-002

## 1. Purpose
Scale an image so its longest side equals a target size, preserving aspect ratio.

## 2. Public API (header first)

```cpp
// include/thumbgen/resize.h
namespace thumbgen {

// Computes the output size for a given input and target longest side.
[[nodiscard]] Size fit_longest_side(Size src, int longest);

// Resizes src into dst. dst's storage is reused if large enough (no allocation in steady state).
void resize_box_into(ImageView src, Size dst_size, Image& dst);

} // namespace thumbgen
```

Usage example (written *before* implementation):
```cpp
Image thumb;                                   // one per worker, reused for every file
auto size = fit_longest_side(decoded.size(), 256);
resize_box_into(decoded.view(), size, thumb);
```

> 💡 **Why:** writing the header and a usage example first exposes awkward APIs before you have
> invested in an implementation.

## 3. Data structures & ownership
- `ImageView` — non-owning `{const std::uint8_t* pixels; int width, height, channels;}`. Must not outlive
  the `Image` it views.
- `Image` — owns pixels in a `std::vector<std::uint8_t>`. Movable, copyable (copy is explicit cost, documented).
- `resize_box_into` takes `ImageView` by value (cheap, 24 bytes) and `Image&` as output parameter
  **so the caller controls the buffer's lifetime and reuse**.

> 💡 **Why:** ownership is visible in the signature: view = borrow, `Image&` = you provide the storage.

## 4. Algorithm
Box filter (average of source pixels covering each destination pixel). Good quality for downscaling,
simple, cache-friendly when iterating rows.

## 5. Performance notes
- Budget: ≤ 2 ms per 12 MP → 256 px resize (keeps total under REQ-N-001 together with decode ≈ 60 ms on 8 threads).
- Row-major iteration over source; no per-pixel allocation; no virtual calls.
- Zero allocations after the first file per worker (verified by test with a counting allocator).

## 6. Test plan
| Test | Type | Traces |
|---|---|---|
| `FitLongestSide.Landscape/Portrait/Square/AlreadySmall` | unit | REQ-F-002 |
| `ResizeBox.SolidColorStaysSolid` | unit | REQ-F-002 |
| `ResizeBox.ReusesBuffer_NoAllocation` | unit | REQ-N-002 |
| `bench_resize_12mp_to_256` | benchmark | REQ-N-001 |

## 7. Alternatives considered
- Bilinear: faster but aliasing when downscaling a lot → rejected.
- Library (stb_image_resize): fine, but resize is small and a good learning target → own implementation;
  revisit if quality complaints appear.
